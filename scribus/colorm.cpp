/****************************************************************************
** Form implementation generated from reading ui file 'Color.ui'
**
** Created: Mon Apr 23 19:09:31 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#include "colorm.h"
#include "colorm.moc"
#include <qvariant.h>
#include <qtooltip.h>
#include <qpixmap.h>
#include <qcolordialog.h>
#include "customfdialog.h"
#include "dcolor.h"
#include "scribusXml.h"
#include "cmykfw.h"

#if (_MSC_VER >= 1200)
 #include "win-config.h"
#else
 #include "config.h"
#endif

extern QPixmap loadIcon(QString nam);

Farbmanager::Farbmanager( QWidget* parent, CListe doco, bool HDoc )
    : QDialog( parent, "dd", true, 0 )
{
		setName( "Farbmanager" );
		DontChange += "White";
		DontChange += "Black";
		DontChange += "Cyan";
		DontChange += "Magenta";
		DontChange += "Yellow";
		HaveDoc = HDoc;
    setSizePolicy(QSizePolicy((QSizePolicy::SizeType)1, (QSizePolicy::SizeType)1, sizePolicy().hasHeightForWidth() ) );
    setMinimumSize( QSize( 300, 260 ) );
    setSizeGripEnabled(true);
    setCaption( tr( "Colors" ) );
  	setIcon(loadIcon("AppIcon.png"));
    Layout2 = new QHBoxLayout( this );
    Layout2->setSpacing( 6 );
    Layout2->setMargin( 11 );
    ListBox1 = new QListBox( this, "ListBox1" );
    ListBox1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)3, ListBox1->sizePolicy().hasHeightForWidth() ) );
    ListBox1->setMinimumSize( QSize( 164, 228 ) );
    ListBox1->setColumnMode( QListBox::FixedNumber );
    Layout2->addWidget( ListBox1 );
    Layout1 = new QVBoxLayout;
    Layout1->setSpacing( 6 );
    Layout1->setMargin( 0 );
    LoadF = new QPushButton( this, "LoadF" );
    LoadF->setText( tr( "Append" ) );
    Layout1->addWidget( LoadF );
    NewF = new QPushButton( this, "NewF" );
    NewF->setText( tr( "New" ) );
    Layout1->addWidget( NewF );
    EditF = new QPushButton( this, "EditF" );
    EditF->setEnabled( false );
    EditF->setText( tr( "Edit" ) );
    EditF->setDefault( true );
    Layout1->addWidget( EditF );
    DupF = new QPushButton( this, "DupF" );
    DupF->setEnabled( false );
    DupF->setText( tr( "Duplicate" ) );
    Layout1->addWidget( DupF );
    DelF = new QPushButton( this, "DelF" );
    DelF->setEnabled( false );
    DelF->setText( tr( "Delete" ) );
    Layout1->addWidget( DelF );
		if (!HaveDoc)
			{
    	Rest = new QPushButton( this, "Rest" );
    	Rest->setText( tr( "Defaults" ) );
    	Layout1->addWidget( Rest );
			}
    SaveF = new QPushButton( this, "SaveF" );
    SaveF->setText( tr( "Save" ) );
    Layout1->addWidget( SaveF );
    CancF = new QPushButton( this, "CancF" );
    CancF->setText( tr( "Cancel" ) );
    CancF->setDefault( true );
    Layout1->addWidget( CancF );
    QSpacerItem* spacer = new QSpacerItem( 0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding );
    Layout1->addItem( spacer );
    Layout2->addLayout( Layout1 );
    Ersatzliste.clear();
    EditColors = doco;
    updateCList();
    // signals and slots connections
		if (!HaveDoc)
    	connect( Rest, SIGNAL( clicked() ), this, SLOT( loadDefaults() ) );
    connect( SaveF, SIGNAL( clicked() ), this, SLOT( accept() ) );
    connect( CancF, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( NewF, SIGNAL( clicked() ), this, SLOT( neueFarbe() ) );
    connect( EditF, SIGNAL( clicked() ), this, SLOT( editFarbe() ) );
    connect( DupF, SIGNAL( clicked() ), this, SLOT( duplFarbe() ) );
    connect( DelF, SIGNAL( clicked() ), this, SLOT( delFarbe() ) );
    connect( LoadF, SIGNAL( clicked() ), this, SLOT( loadFarben() ) );
    connect( ListBox1, SIGNAL( highlighted(QListBoxItem*) ), this, SLOT( selFarbe(QListBoxItem*) ) );
}

void Farbmanager::loadDefaults()
{
  EditColors.clear();
  QString pfadC = PREL;
  QString pfadC2 = pfadC + "/lib/scribus/rgbscribus.txt";
  QFile fiC(pfadC2);
  if (!fiC.exists())
		{
	 	EditColors.insert("White", CMYKColor(0, 0, 0, 0));
  	EditColors.insert("Black", CMYKColor(0, 0, 0, 255));
		EditColors.insert("Blue", CMYKColor(255, 255, 0, 0));
		EditColors.insert("Cyan", CMYKColor(255, 0, 0, 0));
		EditColors.insert("Green", CMYKColor(255, 0, 255, 0));
		EditColors.insert("Red", CMYKColor(0, 255, 255, 0));
		EditColors.insert("Yellow", CMYKColor(0, 0, 255, 0));
		EditColors.insert("Magenta", CMYKColor(0, 255, 0, 0));
		}
	else
		{
		if (fiC.open(IO_ReadOnly))
			{
			QString ColorEn, Cname;
			int Rval, Gval, Bval;
			QTextStream tsC(&fiC);
			ColorEn = tsC.readLine();
			while (!tsC.atEnd())
				{
				ColorEn = tsC.readLine();
				QTextStream CoE(&ColorEn, IO_ReadOnly);
				CoE >> Rval;
				CoE >> Gval;
				CoE >> Bval;
				CoE >> Cname;
				CMYKColor tmp;
				tmp.setColorRGB(Rval, Gval, Bval);
				EditColors.insert(Cname, tmp);
				}
			fiC.close();
			}
		}
	updateCList();
}

void Farbmanager::loadFarben()
{
	QString fileName;
#ifdef HAVE_LIBZ
	CustomFDialog dia(this, tr("Open"), tr("Documents (*.sla *.sla.gz *.scd *.scd.gz);;All Files (*)"));
#else
	CustomFDialog dia(this, tr("Open"), tr("Documents (*.sla *.scd);;All Files (*)"));
#endif
	if (dia.exec() == QDialog::Accepted)
		fileName = dia.selectedFile();
	else
		return;
  if (!fileName.isEmpty())
  	{
  	ScriXmlDoc *ss = new ScriXmlDoc();
  	if (ss->ReadColors(fileName))
  		{
  		CListe LColors = ss->Farben;
			CListe::Iterator it;
			for (it = LColors.begin(); it != LColors.end(); ++it)
				{
				if (!EditColors.contains(it.key()))
					{
    			EditColors.insert(it.key(), it.data());
					}
				}
    	updateCList();
  		}
		delete ss;
  	}
}

void Farbmanager::delFarbe()
{
	if (DontChange.contains(sFarbe))
		return;
	DelColor *dia = new DelColor(this, EditColors, sFarbe, HaveDoc);
	if (dia->exec())
		{
    Ersatzliste.insert(sFarbe, dia->EFarbe);
    EditColors.remove(sFarbe);
    updateCList();
    }
	delete dia;
}

void Farbmanager::duplFarbe()
{
    QString nam;
    Query *dia = new Query(this, "tt", 1, 0, "New Color:", "New Color");
    dia->Answer->setText( tr("Copy of %1").arg(sFarbe));
    if (dia->exec())
    	{
    	nam = dia->Answer->text();
    	while (EditColors.contains(nam))
    		{
    		if (!dia->exec())
    			{
    			delete dia;
    			return;
    			}
    		nam = dia->Answer->text();
    		}
    	EditColors.insert(nam, EditColors[sFarbe]);
			updateCList();
    	}
    delete dia;
}

void Farbmanager::neueFarbe()
{
    QString nam;
    Query *dia = new Query(this, "tt", 1, 0, "New Color:", "New Color");
    dia->Answer->setText(sFarbe);
    if (dia->exec())
    	{
    	nam = dia->Answer->text();
    	while (EditColors.contains(nam) || (nam == "None"))
    		{
    		if (!dia->exec())
    			{
    			delete dia;
    			return;
    			}
    		nam = dia->Answer->text();
    		}
			CMYKColor tmpFarbe = CMYKColor(0, 0, 0, 0);
			CMYKChoose* dia2 = new CMYKChoose(this, tmpFarbe, nam);
			if (!dia2->exec())
				{
    		delete dia;
    		delete dia2;
    		return;
    		}
			tmpFarbe = dia2->Farbe;
			delete dia2;
    	EditColors.insert(nam, tmpFarbe);
			updateCList();
    	}
    delete dia;
}

void Farbmanager::editFarbe()
{
	if (DontChange.contains(sFarbe))
		return;
	CMYKColor tmpFarbe = CMYKColor();
	CMYKChoose* dia = new CMYKChoose(this, EditColors[sFarbe], sFarbe);
	if (!dia->exec())
		{
    delete dia;
    return;
    }
	tmpFarbe = dia->Farbe;
	delete dia;
	EditColors[sFarbe] = tmpFarbe;
	Ersatzliste.insert(sFarbe, sFarbe);
	updateCList();
}

void Farbmanager::selFarbe(QListBoxItem *c)
{
	sFarbe = c->text();
	EditF->setEnabled(true);
	DupF->setEnabled(true);
  DelF->setEnabled(EditColors.count() == 1 ? false : true);
	if (DontChange.contains(sFarbe))
		{
   	EditF->setEnabled(false);
   	DelF->setEnabled(false);
		}
}

void Farbmanager::updateCList()
{
	ListBox1->clear();
	CListe::Iterator it;
	QPixmap pm = QPixmap(30, 15);
	for (it = EditColors.begin(); it != EditColors.end(); ++it)
		{
		pm.fill(EditColors[it.key()].getRGBColor());
		ListBox1->insertItem(pm, it.key());
		}
	DelF->setEnabled(EditColors.count() == 1 ? false : true);
	if (ListBox1->currentItem() == -1)
   	{
   	DupF->setEnabled(false);
   	EditF->setEnabled(false);
    DelF->setEnabled(false);
   	}
	ListBox1->setSelected(ListBox1->currentItem(), false);	
}

