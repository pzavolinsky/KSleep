// The MIT License (MIT)
// 
// Copyright (c) 2014 Patricio Zavolinsky
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
#include "KSleep.h"

#include <kapplication.h>
#include <kdialog.h>
#include <kpassivepopup.h>
#include <kconfig.h>

#include <QMap>
#include <QProcess>
#include <QMouseEvent>
#include <QDBusConnection>
#include <QDesktopWidget>

KSleep::KSleep(KApplication *application) : QDBusAbstractAdaptor(application),
	_config_dir(QDir::home().filePath(".ksleep")),
	_popup_foreground(0xFF, 0xFF, 0xFF), _popup_background(0x21, 0x21, 0x21),
	_popup_height(20), _popup_font_size(18)
{
	// Connect to D-BUS and register as an object:
	QDBusConnection::sessionBus().registerObject("/KSleep", this);
	
	// Default config
	_timeout  = 0;
	_timerId  = 0;
	_halt_cmd = "ksleep-halt.sh";
	
	_trayIcon = new KSystemTrayIcon();
	connect(_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			SLOT(iconActivated(QSystemTrayIcon::ActivationReason)) );
	_trayIcon->hide();
	
	// Read config
	if (_config_dir.exists())
	{
		KConfig config(_config_dir.filePath("ksleep.conf"));
		QMap<QString, QString> options = config.entryMap("Options");
		
		// Halt
		if (options.contains("halt"))
		{
			_halt_cmd = QDir::cleanPath(_config_dir.filePath(options["halt"]));
		}
		
		// Popup Foreground color
		if (options.contains("popupForegroundColor"))
		{
			_popup_foreground.setNamedColor(options["popupForegroundColor"]);
		}
		
		// Popup Background color
		if (options.contains("popupBackgroundColor"))
		{
			_popup_background.setNamedColor(options["popupBackgroundColor"]);
		}
		
		// Popup Height
		if (options.contains("popupHeight"))
		{
			_popup_height = atoi(options["popupHeight"].toLatin1().data());
		}
		
		// Popup Font Size
		if (options.contains("popupFontSize"))
		{
			_popup_font_size = atoi(options["popupFontSize"].toLatin1().data());
		}
	}
}

void KSleep::updateTrayIcon()
{
	if (!_config_dir.exists())
	{
		return;
	}
	
	if (_timeout <= 0)
	{
		_trayIcon->hide();
		return;
	}
	
	if (_timeout < 1*60) // less than one minute
	{
		if (_timeout % 2) // even second
		{
			_trayIcon->setIcon(QIcon(_config_dir.filePath("ksleep-01-on.png")));
		}
		else
		{
			_trayIcon->setIcon(QIcon(_config_dir.filePath("ksleep-01-off.png")));
		}
	}
	else if (_timeout < 15*60) // less than 15 minutes
	{
		_trayIcon->setIcon(QIcon(_config_dir.filePath("ksleep-15.png")));
	}
	else
	{
		_trayIcon->setIcon(QIcon(_config_dir.filePath("ksleep-on.png")));
	}
	
	_trayIcon->show();
	_trayIcon->setToolTip(remaining());
}

QString KSleep::remaining()
{
	int value;
	const char* suffix;
	
	if (_timeout < 60)
	{
		value = _timeout;
		suffix = " secs!";
	}
	else
	{
		value = _timeout / 60;
		suffix = " mins!";
	}
	
	return QString("%1 %2").arg(value).arg(suffix);
}

void KSleep::timerEvent(QTimerEvent* event)
{
	if (!_timerId)
	{
		return;
	}
	
	if (_timeout <= 0)
	{
		halt();
		return;
	}
	
	updateTrayIcon();
	
	if (_timeout < 60 || _timeout == 300 || _timeout == 600 || _timeout == 900)
	{
		popup(remaining(), 1500);
	}
	
	--_timeout;
}

void KSleep::setTimeout(int minutes)
{
	if (_timerId)
	{
		killTimer(_timerId);
		_timerId = 0;
	}
	
	_timeout = minutes*60;
	
	if (minutes != 0)
	{
		timerEvent(NULL); // Fire first update
		
		char data[50];
		sprintf(data, "%d mins", minutes);
		
		popup(data, 1500);

		_timerId = startTimer(1000); // Set 1 second timer
	}

	updateTrayIcon();
}

void KSleep::stop()
{
	setTimeout(0);
}

void KSleep::halt()
{
	popup("Done!", 1500);
	stop();
	QProcess* proc = new QProcess(this);
	proc->start(_halt_cmd);
	if (!proc->waitForStarted())
	{
		printf("KSleep: cannot execute %s\n", _halt_cmd.toAscii().data());
	}
}


void KSleep::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::DoubleClick)
	{
		stop();
	}
}

void KSleep::popup(QString msg, int time)
{
	KPassivePopup *pop = new KPassivePopup;
	pop->setAutoDelete( true );
	pop->setTimeout(time);
	
	QLabel* label = new QLabel(msg, pop);

	QFont font = label->font();
	font.setBold( true );
	font.setPixelSize(_popup_font_size);
	label->setFont( font );
	label->setAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
	
	pop->setView(label);
	
	pop->setFixedSize(QApplication::desktop()->screenGeometry().width(), _popup_height);
	pop->setStyleSheet(QString("color: ")+ _popup_foreground.name()+QString("; background-color: ")+_popup_background.name() );
	
	pop->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
	pop->show();
}

#include "KSleep.moc"
