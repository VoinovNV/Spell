#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QActionGroup>
#include <QString>

#include <string>
#include <vector>

#include <nuspell/dictionary.hxx>
#include <nuspell/finder.hxx>
#include "m_dict.hpp"
#include "dict_settings.hpp"
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

    void on_addAct();
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
    QScopedPointer<M_dict> current_dict;
    QScopedPointer<QStringList> paths_;
    QString custom_dict_path;
    QString cur_word;
    QScopedPointer<Dict_settings> dict_sett;
    void save_file(QString name);
    void load_paths();
    QTextCursor text_cursor;
    bool last_word_flag=false;
};
#endif // MAINWINDOW_HPP
