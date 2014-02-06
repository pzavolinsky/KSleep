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
#ifndef _KSLEEP_H__
#define _KSLEEP_H__

#include <kpassivepopup.h>
#include <kmainwindow.h>
#include <ksystemtrayicon.h>
#include <kapplication.h>

#include <QDir>
#include <QLabel>
#include <QColor>
#include <QDBusAbstractAdaptor>

class KSleep : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.ksleep.DBus.MainApplication")
	public:
		KSleep(KApplication *application);

	public slots:
		Q_NOREPLY void setTimeout(int minutes);
		Q_NOREPLY void stop();
		Q_NOREPLY void halt();
		QString remaining();
		
	protected:
		QDir    _config_dir;
		int     _timeout;
		int     _timerId;
		QString _halt_cmd;
		QColor  _popup_foreground;
		QColor  _popup_background;
		int     _popup_height;
		int     _popup_font_size;
		KSystemTrayIcon* _trayIcon;
		
		void timerEvent(QTimerEvent* event);
		
		void popup(QString msg, int time);
		void updateTrayIcon();
		
	protected slots:
		void iconActivated(QSystemTrayIcon::ActivationReason reason);
};

#endif
