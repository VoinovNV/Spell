#include "mainwindow.hpp"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QTextStream>
#include <QProcess>
#include <nuspell/dictionary.hxx>
#include <nuspell/finder.hxx>
MainWindow::MainWindow(QWidget* parent) :
    QMainWindow{parent},
    ui_{new Ui::MainWindow},
    current_dict{new M_dict},
    paths_{new QStringList},
    dict_sett{new Dict_settings}
{
    ui_->setupUi(this);

    connect(ui_->actionQuit,&QAction::triggered,qApp,&QApplication::quit);
    connect(ui_->actionOpen_File, &QAction::triggered,this,&MainWindow::on_openAct);
    connect(ui_->actionSave_File, &QAction::triggered,this,&MainWindow::on_saveAct);
    connect(ui_->actionSave_As, &QAction::triggered,this,&MainWindow::on_saveAsAct);
    connect(ui_->actionStart, &QAction::triggered,this,&MainWindow::on_startAct);
    connect(ui_->NextButton, &QPushButton::clicked,this,&MainWindow::on_nextAct);
    connect(ui_->ReplaceButton, &QPushButton::clicked,this,&MainWindow::on_replaceAct);
    connect(ui_->StopButton, &QPushButton::clicked,this,&MainWindow::on_stopAct);
    connect(ui_->AddButton, &QPushButton::clicked,this,&MainWindow::on_addAct);
    connect(ui_->actionSet_dictionary, &QAction::triggered,this,&MainWindow::on_chpath);
    ui_->SPELL_WIDGET->hide();
    load_paths();
}
void MainWindow::load_paths(){
    custom_dict_path=CUSTOM_DICTIONARY_PATH;
    std::vector<std::pair<std::string, std::string>> dict_list;
    nuspell::search_default_dirs_for_dicts(dict_list);
    for(std::size_t i=0; i<dict_list.size(); i++) paths_->append(dict_list[i].second.data());
    paths_->append(custom_dict_path);
    dict_sett->paths_=paths_.data();
}

MainWindow::~MainWindow() = default;

void MainWindow::on_addAct(){
    if(cur_word.isEmpty()) return;
    QFile file(custom_dict_path+".dic");
    if (!file.open(QFile::WriteOnly | QFile::Text |QIODevice::Append)) {
        QMessageBox::warning(this, "Error!",
                             file.errorString());
        return;
    }
    QTextStream out(&file);
    out << cur_word<<'\n';

}
void MainWindow::on_openAct()
{
    filename = QFileDialog::getOpenFileName(this);
    if(!filename.isEmpty()){
        QFile file(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, "Error!",
                                 file.errorString());
            return;
        }
        QTextStream in(&file);
        ui_->textEdit->setPlainText(in.readAll());

    }
}

void MainWindow::save_file(QString name){
    QFile file(name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error!",
                             file.errorString());
        return;
    }
    QTextStream out(&file);
    out << ui_->textEdit->toPlainText();

}
void MainWindow::on_saveAct(){
    if (filename.isEmpty()) {
        return on_saveAsAct();
    }
    else {
        return save_file(filename);
    };
}
void MainWindow::on_saveAsAct(){
    QString FN = QFileDialog::getSaveFileName(this);
    if (FN.isEmpty()) return;
    return save_file(FN);
}
void MainWindow::on_chpath(){
    on_stopAct();
    dict_sett->update_list();
    dict_sett->setModal(true);
    dict_sett->exec();

}
void MainWindow::on_startAct(){
    try{
        current_dict->load(*paths_);
    }
    catch(nuspell::Dictionary_Loading_Error& err){
        QMessageBox::warning(this,"Error","No Dictionary");
        return;
    }
    ui_->SPELL_WIDGET->show();
    ui_->listWidget->clear();
    text_cursor = QTextCursor(ui_->textEdit->document());
    return on_nextAct();
}

void MainWindow::on_nextAct(){
    ui_->listWidget->clear();
    if(last_word_flag) {text_cursor.movePosition(QTextCursor::Start); last_word_flag=false;}
    auto sugs = std::vector<std::string>();
    QString word;
    do{
        text_cursor.select(text_cursor.WordUnderCursor);
        word = text_cursor.selectedText();
        if (!current_dict->spell(word.toStdString())){
            ui_->textEdit->setTextCursor(text_cursor);
            cur_word=word;
            QString r_="<span style='background-color: red;'>"+word+"</p>";
            text_cursor.insertHtml(r_);
            current_dict->suggest(word.toStdString(), sugs);
            if (!sugs.empty()){
                for(auto i : sugs){
                    QString i_(i.data());
                    ui_->listWidget->addItem(i_);
                }
            }
            if(!text_cursor.movePosition(QTextCursor::NextWord)) last_word_flag=true;
            break;
        }
        if(!text_cursor.movePosition(QTextCursor::NextWord)) {last_word_flag=true; return;}
    } while(true);
}

void MainWindow::on_replaceAct(){
    if(!ui_->listWidget->count() || ui_->listWidget->currentRow()==-1) return;
    QString r = ui_->listWidget->currentItem()->text();
    QString r_="<span style='background-color: none;'>"+r+"</p>";
    if(!last_word_flag) text_cursor.movePosition(QTextCursor::PreviousWord);
    text_cursor.select(text_cursor.WordUnderCursor);
    text_cursor.removeSelectedText();
    text_cursor.insertHtml(r_);
    return on_nextAct();

}
void MainWindow::on_stopAct(){
    ui_->listWidget->clear();
    ui_->SPELL_WIDGET->hide();
}
