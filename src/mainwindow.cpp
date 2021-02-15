#include "mainwindow.hpp"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QTextStream>

#include <nuspell/dictionary.hxx>
#include <nuspell/finder.hxx>
MainWindow::MainWindow(QWidget* parent) :
    QMainWindow{parent},
    ui_{new Ui::MainWindow},
    paths_{new QStringList},
    dict_sett{new Dict_settings}

{
    ui_->setupUi(this);

    cur_pos=0;
    connect(ui_->actionQuit,&QAction::triggered,qApp,&QApplication::quit);
    connect(ui_->actionOpen_File, &QAction::triggered,this,&MainWindow::on_openAct);
    connect(ui_->actionSave_File, &QAction::triggered,this,&MainWindow::on_saveAct);
    connect(ui_->actionSave_As, &QAction::triggered,this,&MainWindow::on_saveAsAct);
    connect(ui_->actionStart, &QAction::triggered,this,&MainWindow::on_startAct);
    //connect(ui_->actionPath_to_dictionary,&QAction::triggered,this,&MainWindow::on_settAct);
    connect(ui_->NextButton, &QPushButton::clicked,this,&MainWindow::on_nextAct);
    connect(ui_->ReplaceButton, &QPushButton::clicked,this,&MainWindow::on_replaceAct);
    connect(ui_->StopButton, &QPushButton::clicked,this,&MainWindow::on_stopAct);
    connect(ui_->actionSet_dictionary, &QAction::triggered,this,&MainWindow::on_chpath);
    ui_->SPELL_WIDGET->hide();

    std::vector<std::pair<std::string, std::string>> dict_list;
    nuspell::search_default_dirs_for_dicts(dict_list);
    for(std::size_t i=0; i<dict_list.size(); i++) paths_->append(dict_list[i].second.data());
    auto dict_name_and_path = nuspell::find_dictionary(dict_list, "en_US");
    if (dict_name_and_path == std::end(dict_list)) {
        QMessageBox::warning(this,"Error","No default dictionary");
    }
    else{
        ch_dict_path=dict_name_and_path->second;
    }
    dict_sett->paths_=paths_.data();
    dict_sett->ch_path=&ch_dict_path;
}

MainWindow::~MainWindow() = default;
/*void MainWindow::on_settAct(){
    filename = QFileDialog::getOpenFileName(this);
    if(!filename.isEmpty()){
        paths_->append(filename);
    }
    else QMessageBox::warning(this, "Error!", "Error in path");
}*/
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
        ui_->plainTextEdit->setPlainText(in.readAll());

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
    out << ui_->plainTextEdit->toPlainText();
}
void MainWindow::on_saveAct(){
    if (filename.isEmpty()) {
        return on_saveAsAct();
    } else {
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
        current_dict = nuspell::Dictionary::load_from_path(ch_dict_path);
    }
    catch(nuspell::Dictionary_Loading_Error& err){
        QMessageBox::warning(this,"Error","No Dictionary");
        return;
    }

    ui_->SPELL_WIDGET->show();
    ui_->listWidget->clear();

    //Word_list = ui_->plainTextEdit->toPlainText().split(QRegExp("\\W+"), QString::SkipEmptyParts);
    //cur_pos=-1;
    //
    text_cursor = QTextCursor(ui_->plainTextEdit->document());
    return on_nextAct();
}

void MainWindow::on_nextAct(){
/*    ui_->listWidget->clear();
    cur_pos++;
    if(cur_pos>=Word_list.size()) cur_pos=0;
    if(Word_list.size()==0) return;

    for (int i=cur_pos;i<Word_list.size();i++) {
        auto Word=Word_list[cur_pos];
        if (!current_dict.spell(Word.toStdString())){
            QTextCursor text_cursor = QTextCursor(ui_->plainTextEdit->document()->find(Word_list[cur_pos]));
            QString r_="<span style='background-color: red;'>"+Word+"</p>";
            text_cursor.insertHtml(r_);
            current_dict.suggest(Word.toStdString(), sugs);
            if (!sugs.empty()){
                for(auto i : sugs){
                    QString i_(i.data());
                    ui_->listWidget->addItem(i_);
                }
            }
            break;
        }
        else cur_pos++;
    }
    */
    ui_->listWidget->clear();

    auto sugs = std::vector<std::string>();
QString word;
    do{

    text_cursor.select(text_cursor.WordUnderCursor);
        word = text_cursor.selectedText();
        if (!(word.length()==1 && !word[0].isLetter()) && (!current_dict.spell(word.toStdString()))){
            QString r_="<span style='background-color: red;'>"+word+"</p>";
            //QString r_="<span style='text-decoration: underline; text-decoration-style: wavy; text-decoration-color: red;'>"+word+"</p>";
            //
            //QString r_="<span style=\"color:green; font-family:Verdana; font-size:150%; font-weight:bold; font-style:italic; font-variant:small-caps\">"+word+"</p>";

            text_cursor.insertHtml(r_);
            current_dict.suggest(word.toStdString(), sugs);
            if (!sugs.empty()){
                for(auto i : sugs){
                    QString i_(i.data());
                    ui_->listWidget->addItem(i_);
                }
           }
            if(!text_cursor.movePosition(QTextCursor::NextWord)) last_word_flag=true;
           break;
        }
        if(!text_cursor.movePosition(QTextCursor::NextWord)) return;

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
