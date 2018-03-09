/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#ifndef QNECONNECTION_H
#define QNECONNECTION_H

#include <QGraphicsPathItem>
#include <QtXml/QDomDocument>
#include <QtWidgets/QAction>

namespace calenhad {
	namespace notification {
		class QNotificationFactory;
	}
	namespace nodeedit {
		class Port;
		class Connection : public QGraphicsPathItem {

		public:
			enum {
				Type = QGraphicsItem::UserType + 2
			};

			Connection (QGraphicsItem* parent = 0);

			~Connection ();

			void setPos1 (const QPointF& p);

			void setPos2 (const QPointF& p);

			void setPort1 (Port* p);

			void setPort2 (Port* p);

			void updatePosFromPorts ();

			void updatePath ();

			Port* port1 () const;

			Port* port2 () const;

			bool canDrop;

			int type () const { return Type; }

			void serialise (QDomDocument& doc);

			void inflate (const QDomDocument& doc);

			Port* otherEnd (Port* port);

			void mousePressEvent (QGraphicsSceneMouseEvent* e) override;

		private:

			QPointF pos1;
			QPointF pos2;
			Port* m_port1;
			Port* m_port2;


			QMenu* _connectionContextMenu;
            QAction* _deleteConnectionAction;
        };
	}
}

#endif // QNECONNECTION_H
