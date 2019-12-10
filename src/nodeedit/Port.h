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

#ifndef QNEPORT_H
#define QNEPORT_H

#include <QGraphicsPathItem>
#include <QtWidgets/QLabel>

namespace calenhad {
	namespace module {
		class Module;
	}
	namespace nodeedit {
		class NodeBlock;
		class Connection;
		class EditableLabel;
		class NodeNameValidator;


		class Port : public QObject, public QGraphicsPathItem {
		Q_OBJECT


		public:

            enum PortHighlight {
                NONE, CAN_CONNECT, CONNECTED
            };
			enum {
				Type = QGraphicsItem::UserType + 1
			};
			enum {
				ControlPort = 0, InputPort = 1, OutputPort = 2
			};

			Port (int type, int index, const QString& name, const QString& label, const double& defaultValue = 0.0, const bool& required = false);

			~Port ();

			void setBlock (NodeBlock*);

			int index ();

			int radius ();

			QVector<Connection*>& connections ();

			void initialise ();

			QString& portName ();

			int portType () const { return _portType; }

			bool hasConnection ();

			int type () const { return Type; }

			NodeBlock* block () const;

			void addConnection (Connection* c);

			void removeConnection (Connection* c);

			virtual QRectF boundingRect () const;

			bool isConnected (Port*);

			calenhad::module::Module* owner ();

			void invalidateRenders ();

			void setHighlight (const PortHighlight& highlight);

			calenhad::module::Module* source();

			Q_PROPERTY (QString name READ portName WRITE setName MEMBER	_portName);
            QMenu* connectMenu ();

            bool isRequired();

            const QString& porLabel ();
		public slots:

			void setName (const QString& n);

			void nameChangeRequested (const QString& value);

		signals:

			void connected (Connection* c);

			void disconnected (Port* port);

		protected:
			QVariant itemChange (GraphicsItemChange change, const QVariant& value);

		private:
			NodeBlock* _block;
			QString _portName;
            QString _portLabel;
			int _radius;
			int _margin;
			QVector<Connection*> _connections;
			int _portType;
			int _index;
			bool _required;
			quint64 _ptr;


            NodeNameValidator* _nameValidator;
			bool _hasDefaultValue;
			double _defaultValue;

            QMenu* _connectMenu;



            void paint (QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

        };
	}
}

#endif // QNEPORT_H
