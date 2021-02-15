#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QActionGroup>
#include <QString>

#include <string>
#include <vector>

#include <nuspell/dictionary.hxx>
#include <nuspell/finder.hxx>

#include "dict_settings.h"
#include "./ui_mainwindow.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:

    void on_openAct();
    void on_saveAct();
    void on_startAct();
    void on_saveAsAct();
    void on_nextAct();
    void on_replaceAct();
    void on_stopAct();
    void on_chpath();

private:

    QScopedPointer<Ui::MainWindow> ui_;

    QString filename;
    nuspell::Dictionary current_dict;
    QScopedPointer<QStringList> paths_;
    std::string ch_dict_path;
    QScopedPointer<Dict_settings> dict_sett;
    void save_file(QString name);
    QTextCursor text_cursor;
    bool last_word_flag=false;
};
#endif // MAINWINDOW_HPP
