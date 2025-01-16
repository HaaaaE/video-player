#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QPushButton>
#include<QLabel>
#include<QVBoxLayout>
#include<QTextEdit>
#include<QMenu>
#include<QActionGroup>
#include<QFileSystemModel>
#include<QFileDialog>
#include<QMimeData>
#include<QDropEvent>
#include<QDragEnterEvent>
#include<QPainter>
#include<QAbstractItemView>
#include<QPixmap>
#include<QTreeWidgetItem>
#include<QDataStream>
#include<QPixmap>
#include<QMessageBox>
#include<QSizePolicy>
#include<QTreeWidget>
#include<QInputDialog>
#include"aianalysis.h"
#include<QDataStream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralwidget->setAttribute(Qt::WA_Hover,true);
    ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    setAcceptDrops(true);
    ui->pushButton->setStyleSheet("QPushButton{background-color:transparent;}");
    ui->pushButton_2->setStyleSheet("QPushButton{background-color:transparent;}");
    ui->pushButton_3->setStyleSheet("QPushButton{background-color:transparent;}");

    ui->pushButton_4->setStyleSheet("QPushButton{background-color:transparent;}");
    ui->showfull->setStyleSheet("QPushButton{background-color:transparent;}");
    ui->speedbutton->setStyleSheet("QToolButton{background-color:transparent;}");
    ui->label_3->setStyleSheet("QLabel{background-color:transparent;}");

    auto menu=new QMenu("璁?);
    auto menutheme=new QMenu("涓婚");
    auto themegroup=new QActionGroup(menutheme);
    themegroup->addAction(ui->actthemeblack);
    themegroup->addAction(ui->actthemewhite);
    menutheme->addAction(ui->actthemeblack);
    menutheme->addAction(ui->actthemewhite);
    auto menubofang=new QMenu("鎾斁");
    auto menuhot=new QMenu("鐑敭");
    menu->addMenu(menutheme);
    menu->addMenu(menubofang);
    menu->addMenu(menuhot);
    ui->menuwidget->setMenu(menu);

    auto bofanggroup=new QActionGroup(menubofang);
    menubofang->addAction(ui->actrandom);
    menubofang->addAction(ui->actxun);
    menubofang->addAction(ui->actshun);
    bofanggroup->addAction(ui->actrandom);
    bofanggroup->addAction(ui->actxun);
    bofanggroup->addAction(ui->actshun);
    ui->toolButton_2->setMenu(menubofang);


    auto menuspeed=new QMenu("鍊嶉€熸挱鏀?);
    auto speedgroup=new QActionGroup(menuspeed);
    menuspeed->addAction(ui->action1_0);
    menuspeed->addAction(ui->action1_5);
    menuspeed->addAction(ui->action0_5);
    menuspeed->addAction(ui->action2_0);
    speedgroup->addAction(ui->action1_0);
    speedgroup->addAction(ui->action1_5);
    speedgroup->addAction(ui->action2_0);
    speedgroup->addAction(ui->action0_5);
    ui->speedbutton->setMenu(menuspeed);

     ui->treeWidget->clear();
    QTreeWidgetItem *header = new QTreeWidgetItem();
    header->setText(0,"name");
    header->setText(1,"lastread");
    header->setTextAlignment(0,Qt::AlignHCenter|Qt::AlignVCenter);
    header->setTextAlignment(1,Qt::AlignHCenter|Qt::AlignVCenter);
    ui->treeWidget->setHeaderItem(header);
    loadTreeWidget();

    ui->centralwidget->installEventFilter(this);
    ui->treeWidget->installEventFilter(this);

    contextMenu=new QMenu(this);
    contextMenu->addAction(ui->delete_2);
    contextMenu->addAction(ui->actlike);
    contextMenu->addAction(ui->rename);
    connect(ui->treeWidget,&QTreeWidget::customContextMenuRequested,this,
            &MainWindow::on_treeWidget_customContextMenuRequested);


    ui->treeWidget->setDragEnabled(true);
    ui->treeWidget->setAcceptDrops(true);
    ui->treeWidget->setDefaultDropAction(Qt::CopyAction);
    ui->treeWidget->setDragDropMode(QAbstractItemView::DragDrop);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        QString filename=event->mimeData()->urls().at(0).fileName();
        QFileInfo fileinfo(filename);
        QString ex=fileinfo.suffix().toUpper();
        if(ex=="MOV"||ex=="JPG")
        {
            event->acceptProposedAction();//鎺ュ彈杩欎釜涓滆タ杩涙潵
        }
        else
        {
            event->ignore();
        }
    }
    else
        event->ignore();
}


void MainWindow::on_action1_0_triggered(bool checked)
{
    if(checked)
    {
        ui->speedbutton->setText("X1.0");
    }
    //鍑芥暟
}


void MainWindow::on_action1_5_triggered(bool checked)
{
    if(checked)
    {
        ui->speedbutton->setText("X1.5");
    }
    //鍑芥暟
}


void MainWindow::on_action0_5_triggered(bool checked)
{
    if(checked)
    {
        ui->speedbutton->setText("X0.5");
    }
    //鍑芥暟
}


void MainWindow::on_action2_0_triggered(bool checked)
{
    if(checked)
    {
        ui->speedbutton->setText("X2.0");
    }
    //鍑芥暟
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->centralwidget)
    {
        if(event->type()==QEvent::HoverEnter)
        {
            ui->progressbar->setHidden(false);
        }
        if(event->type()==QEvent::HoverLeave)
        {
            ui->progressbar->setHidden(true);
        }
    }
    if(watched==ui->treeWidget)
    { if(event->type()==QEvent::KeyPress)
        {
            QKeyEvent *keyevent=static_cast<QKeyEvent*>(event);
            if(keyevent->key()!=Qt::Key_Delete)
                return QWidget::eventFilter(watched,event);
            if(ui->treeWidget->currentItem()->type()==ittop)
            {
                return QWidget::eventFilter(watched,event);
            }
            QString dlgtitle="鍒犻櫎";
            QString strinfo="鏄惁鍒犻櫎";
            int result=QMessageBox::question(this,dlgtitle,strinfo,
                                               QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,
                                               QMessageBox::NoButton);
            if(result==QMessageBox::Yes)
            {
                QTreeWidgetItem *curitem=ui->treeWidget->currentItem();
                if(curitem->parent()!=nullptr){
                    curitem->parent()->removeChild(curitem);
                }
            }
            else if(result==QMessageBox::No)
                return QWidget::eventFilter(watched,event);
            else if(result==QMessageBox::Cancel)
                return QWidget::eventFilter(watched,event);
        }
        return QWidget::eventFilter(watched,event);
    }
    return true;
}
void MainWindow::on_addfolder_triggered()
{
    QString dir=QFileDialog::getExistingDirectory();
    if(dir.isEmpty())return;
    QTreeWidgetItem *paritem,*item;
    item=ui->treeWidget->currentItem();
    if(item->type()==itmusic|item->type()==itvedio)
    {
        paritem=item->parent();
    }
    else
        paritem=item;

    QTreeWidgetItem *folderItem = new QTreeWidgetItem(itgroup);
    folderItem->setText(0, dir);
    folderItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    paritem->addChild(folderItem);
    QDir directory(dir);
    QFileInfoList fileList = directory.entryInfoList(QStringList("*.MOV"), QDir::Files | QDir::NoDotAndDotDot);
    foreach (QFileInfo fileInfo, fileList)
    {
        // 涓烘瘡涓?.mp3 鏂囦欢鍒涘缓涓€涓柊鐨?QTreeWidgetItem
        QTreeWidgetItem *fileItem = new QTreeWidgetItem(itvedio);
        fileItem->setText(0, fileInfo.fileName());
        fileItem->setText(1,fileInfo.lastRead().toString(("yyyy-MM-dd")));
        fileItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        folderItem->addChild(fileItem);
    }
    paritem->setExpanded(true);
}



void MainWindow::on_addfile_triggered()
{
    QStringList files=QFileDialog::getOpenFileNames(this,"閫夋嫨鏂囦欢","","瑙嗛鏂囦欢(*.MOV *.JPG)");
    if(files.isEmpty())return;

    QTreeWidgetItem *paritem,*item;
    item=ui->treeWidget->currentItem();
    if(item->type()==itmusic|item->type()==itvedio)
    {
        paritem=item->parent();
    }
    else
        paritem=item;
    for(int i=0;i<files.size();i++)
    {
        //鏂囦欢杞藉叆
        QString filename=files.at(i);
        QFileInfo fileinfo(filename);
        QString nodetext=fileinfo.fileName();
        bool skip=false;
        for(int j=0;j<history->childCount();j++)
        {
            if(history->child(j)->text(0)==nodetext)
            {
                QMessageBox::information(this,"鎻愮ず",QString("鏂囦欢%1閲嶅锛屽凡璺宠繃").arg(nodetext));
                skip=true;
                break;
            }
        }
        if(skip) continue;
        QTreeWidgetItem *item=new QTreeWidgetItem(itvedio);
        item->setText(0,nodetext);
        item->setText(1,fileinfo.lastRead().toString(("yyyy-MM-dd")));
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        item->setData(0,Qt::UserRole,QVariant(filename));
        paritem->addChild(item);
        //鍥剧墖鏄剧ず
        m_pixmap.load(filename);
        int w=ui->label->width();
        int realw=m_pixmap.width();
        m_ratio=float(w)/realw;
        QPixmap pix=m_pixmap.scaledToWidth(w);
        ui->label->setPixmap(pix);
        width=w;
    }
    paritem->setExpanded(true);
}
void MainWindow::dropEvent(QDropEvent *event)
{
    QString filename=event->mimeData()->urls().at(0).path();
    filename=filename.right(filename.length()-1);
    QTreeWidgetItem *paritem,*item;
    item=ui->treeWidget->currentItem();
    if(item->type()==itmusic|item->type()==itvedio)
    {
        paritem=item->parent();
    }
    else
        paritem=item;

    QFileInfo fileinfo(filename);
    QString nodetext=fileinfo.fileName();
    for(int j=0;j<history->childCount();j++)
    {
        if(history->child(j)->text(0)==nodetext)
        {
            QMessageBox::information(this,"鎻愮ず",QString("鏂囦欢%1閲嶅锛屽凡璺宠繃").arg(nodetext));
            return ;
        }
    }
    QTreeWidgetItem *item1=new QTreeWidgetItem(itvedio);
    item1->setText(0,nodetext);
    item1->setText(1,fileinfo.lastRead().toString(("yyyy-MM-dd")));
    item1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    item1->setData(0,Qt::UserRole,QVariant(filename));
    m_pixmap.load(filename);
    int w=ui->label->width();
    int realw=m_pixmap.width();
    m_ratio=float(w)/realw;
    QPixmap pix=m_pixmap.scaledToWidth(w);
    ui->label->setPixmap(pix);
    width=w;
    paritem->addChild(item1);
    paritem->setExpanded(true);

}
// void MainWindow::paintEvent(QPaintEvent *event)
// {
//     QPainter painter(this);
//     painter.drawPixmap(0,0,1000,this->height(),QPixmap(":/resource/source/1.JPG"));
// }


void MainWindow::on_pushButton_2_clicked(bool checked)
{
    if(checked==true)
    {
        ui->widget_2->setHidden(false);
        int w=width;
        int realw=m_pixmap.width();
        m_ratio=float(w)/realw;
        QPixmap pix=m_pixmap.scaledToWidth(w);
        ui->label->setPixmap(pix);
    }
    else
    {
        ui->widget_2->setHidden(true);
        int w=ui->splitter_2->width()-20;
        int realw=m_pixmap.width();
        m_ratio=float(w)/realw;
        QPixmap pix=m_pixmap.scaledToWidth(w);
        ui->label->setPixmap(pix);
    }
}


void MainWindow::on_actshun_triggered(bool checked)
{
    if(checked)
    {
        ui->toolButton_2->setText("椤哄簭鎾斁");
    }
}

void MainWindow::on_actxun_triggered(bool checked)
{
    if(checked)
    {
        ui->toolButton_2->setText("寰幆鎾斁");
    }
}

void MainWindow::on_actrandom_triggered(bool checked)
{
    if(checked)
    {
        ui->toolButton_2->setText("闅忔満鎾斁");
    }
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current==nullptr) return;
    if(current->type()==itvedio)
    {
        ui->pushButton_6->setEnabled(true);
        if(state[current])
        {
            ui->pushButton_6->setChecked(true);
        }
        else
        {
            ui->pushButton_6->setChecked(false);
        }
        QString filename=current->data(0,Qt::UserRole).toString();
        m_pixmap.load(filename);

        int w=ui->label->width()-10;
        int realw=m_pixmap.width();
        m_ratio=float(w)/realw;
        QPixmap pix=m_pixmap.scaledToWidth(w);
        ui->label->setPixmap(pix);
        width=ui->label->width();
    }
    else if(current->type()==ittop|current->type()==itgroup)
    {
        ui->pushButton_6->setEnabled(false);
    }
}


void MainWindow::on_pushButton_clicked()
{
    analysis=new AIanalysis(this);
    analysis->setAttribute(Qt::WA_DeleteOnClose);
    analysis->setWindowFlag(Qt::WindowStaysOnBottomHint);
    connect(analysis,&AIanalysis::changeenanbel,this,&MainWindow::do_setenabled);
    connect(analysis,&AIanalysis::theme,this,&MainWindow::changetheme);
    analysis->show();
    analysis->setModal(false);
}

void MainWindow::do_setenabled(bool en)
{
    ui->treeWidget->setEnabled(en);
}

void MainWindow::changetheme(int n)
{

    switch (n) {
    case musicnow:
        ui->label->setText("鏇存崲浜嗕笌褰撳墠闊充箰鍖归厤鐨勪富棰?);
        break;
    case musiclike:
        ui->label->setText("鏇存崲浜嗕笌浣犲枩娆㈢殑闊充箰鍖归厤鐨勪富棰?);
        break;
    case videonow:
        ui->label->setText("鏇存崲浜嗕笌褰撳墠瑙嗛鍖归厤鐨勪富棰?);
        break;
    case videolike:
        ui->label->setText("鏇存崲浜嗕笌浣犲枩娆㈢殑瑙嗛鍖归厤鐨勪富棰?);
        break;
    }
    analysis->close();
}
void MainWindow::loadTreeWidget()
{
    QFile file("treewidget.state");
    if(!file.open(QIODevice::ReadOnly))
    {
        QTreeWidgetItem *history=new QTreeWidgetItem(ittop);
        history->setText(0,"鎾斁鍒楄〃");
        history->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        like=new QTreeWidgetItem(ittop);
        like->setText(0,"鏀惰棌澶?);
        like->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        ui->treeWidget->addTopLevelItem(history);
        ui->treeWidget->addTopLevelItem(like);
        ui->treeWidget->setCurrentItem(history);
    }

    QDataStream in(&file);
    QByteArray headerState;
    int toplevelitemcount;

    in>>headerState;
    in>>toplevelitemcount;
    in>>likelist;

    ui->treeWidget->header()->restoreState(headerState);

    // 娓呯┖鏍戝舰鎺т欢锛屼絾淇濈暀鍘熸湁鐨勯《绾ч」
    history = nullptr;
    like = nullptr;
    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
        if (item->text(0) == "鎾斁鍒楄〃") {
            history = item;
        } else if (item->text(0) == "鏀惰棌澶?) {
            like = item;
        }
    }


    for(int i=0;i<toplevelitemcount;i++)
    {
        QTreeWidgetItem *item=ui->treeWidget->topLevelItem(i);
        loaditem(in,item);
        if (item->text(0) != "鎾斁鍒楄〃" && item->text(0) != "鏀惰棌澶?) {

            ui->treeWidget->addTopLevelItem(item);
        }
    }
    file.close();
}
void MainWindow::loaditem(QDataStream &in,QTreeWidgetItem *item)
{
    QString text,filepath;

    int childCount;
    Qt::ItemFlags flags;
    in>>text;
    in>>childCount;
    in>>flags; // 璇诲彇椤圭殑灞炴€?
    in>>filepath;
    QFileInfo fileinfo(filepath);
    item->setText(0,text);
    if(likelist.contains(text))
    {
        state[item]=true;
    }
    item->setText(1,fileinfo.lastRead().toString("yyyy-MM-dd"));
    item->setFlags(flags);
    item->setData(0,Qt::UserRole,QVariant(filepath));

    for(int j=0;j<childCount;j++)
    {
        QTreeWidgetItem *child=new QTreeWidgetItem(itvedio);
        loaditem(in,child);
        item->addChild(child);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QFile file("treewidget.state");
    if (!file.open(QIODevice::WriteOnly))
        return;

    QDataStream out(&file);
    QSet<QString> likelist;
    out << ui->treeWidget->header()->saveState(); // 淇濆瓨琛ㄥご鐘舵€?
    out << ui->treeWidget->topLevelItemCount();   // 淇濆瓨椤剁骇椤圭殑鏁伴噺
    for(int i=0;i<like->childCount();i++)
    {
        QTreeWidgetItem *child=like->child(i);
        likelist.insert(child->text(0));
    }
    out<<likelist;

    for (int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
        saveItem(out, item); // 閫掑綊淇濆瓨姣忎釜椤?
    }

    file.close();
    event->accept();
}
void MainWindow::saveItem(QDataStream &out, QTreeWidgetItem *item)
{
    out << item->text(0) << item->childCount() << item->flags()<<item->data(0,Qt::UserRole).toString(); // 淇濆瓨椤圭殑灞炴€?

    for (int j = 0; j < item->childCount(); ++j) {
        QTreeWidgetItem *child = item->child(j);
        saveItem(out, child); // 閫掑綊淇濆瓨瀛愰」
    }
}


void MainWindow::on_treeWidget_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->treeWidget->itemAt(pos);

    if (item)
    {
            QPoint globalPos = ui->treeWidget->mapToGlobal(pos);
            contextMenu->exec(globalPos);
            if(state[item])
            {
                ui->actlike->setText("鍙栨秷鏀惰棌");
                ui->pushButton_6->setChecked(true);
            }
            else
            {
                ui->actlike->setText("娣诲姞鏀惰棌");
                ui->pushButton_6->setChecked(false);
            }

    }
}



void MainWindow::on_delete_2_triggered()
{
    if(ui->treeWidget->currentItem()->type()==ittop)
    {
        return ;
    }
    QString dlgtitle="鍒犻櫎";
    QString strinfo="鏄惁鍒犻櫎";
    int result=QMessageBox::question(this,dlgtitle,strinfo,
                                       QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,
                                       QMessageBox::NoButton);
    if(result==QMessageBox::Yes)
    {
        QTreeWidgetItem *curitem=ui->treeWidget->currentItem();
        if(curitem->parent()!=nullptr){
            curitem->parent()->removeChild(curitem);
        }
    }
    else if(result==QMessageBox::No)
        return ;
    else if(result==QMessageBox::Cancel)
        return ;
}

void MainWindow::on_pushButton_6_clicked()
{
    QTreeWidgetItem *paritem,*item;
    item=ui->treeWidget->currentItem();
    paritem=like;
    if(state[item]==false)

    {
        //鏂囦欢杞藉叆
        QString filename=item->data(0,Qt::UserRole).toString();
        QFileInfo fileinfo(filename);
        QString nodetext=fileinfo.fileName();
        QTreeWidgetItem *item1=new QTreeWidgetItem(itvedio);
        item1->setText(0,nodetext);
        item1->setText(1,fileinfo.lastRead().toString(("yyyy-MM-dd")));
        item1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        item1->setData(0,Qt::UserRole,QVariant(filename));
        paritem->addChild(item1);
        paritem->setExpanded(true);
        state[item]=true;
        state[item1]=true;
    }
    else
    {
        for(int i=0;i<history->childCount();i++)
        {
            QTreeWidgetItem *child=history->child(i);
            if(child->text(0)==item->text(0))
            {
                state[child]=false;
            }
        }
        for(int i=0;i<like->childCount();i++)
        {
            QTreeWidgetItem *child=like->child(i);
            if(child->text(0)==item->text(0))
            {
                child->parent()->removeChild(child);
            }
        }
    }
}
void MainWindow::on_actlike_triggered()
{
    QTreeWidgetItem *paritem,*item;
    item=ui->treeWidget->currentItem();
    paritem=like;
    if(state[item]==false)

    {
        //鏂囦欢杞藉叆
        QString filename=item->data(0,Qt::UserRole).toString();
        QFileInfo fileinfo(filename);
        QString nodetext=fileinfo.fileName();
        QTreeWidgetItem *item1=new QTreeWidgetItem(itvedio);
        item1->setText(0,nodetext);
        item1->setText(1,fileinfo.lastRead().toString(("yyyy-MM-dd")));
        item1->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        item1->setData(0,Qt::UserRole,QVariant(filename));
        paritem->addChild(item1);
        paritem->setExpanded(true);
        state[item]=true;
        state[item1]=true;
    }
    else
    {
        for(int i=0;i<history->childCount();i++)
        {
            QTreeWidgetItem *child=history->child(i);
            if(child->text(0)==item->text(0))
            {
                state[child]=false;
            }
        }
        for(int i=0;i<like->childCount();i++)
        {
            QTreeWidgetItem *child=like->child(i);
            if(child->text(0)==item->text(0))
            {
                child->parent()->removeChild(child);
            }
        }
    }
}


void MainWindow::on_rename_triggered()
{
    QTreeWidgetItem *item = ui->treeWidget->currentItem();
    if (!item) return;

    QString filename = item->data(0, Qt::UserRole).toString();
    QFileInfo fileInfo(filename);
    QString extension = fileInfo.suffix(); // 鑾峰彇鏂囦欢鎵╁睍鍚?
    QString newname = QInputDialog::getText(this, "閲嶅懡鍚?, "璇疯緭鍏ユ柊鍚嶇О", QLineEdit::Normal, item->text(0));
    if (!newname.isEmpty()) {
        {
            QMessageBox::warning(this, "閲嶅懡鍚嶅け璐?, "鏃犳硶閲嶅懡鍚嶆枃浠躲€傝妫€鏌ユ枃浠跺悕鏄惁鏈夋晥鎴栨枃浠舵槸鍚﹁鍗犵敤銆?);
        }
    }
}
