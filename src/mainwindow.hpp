#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QActionGroup>
#include <QString>

#include <string>
#include <vector>

#include <nuspell/dictionary.hxx>
#include <nuspell/finder.hxx>

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
    void on_settAct();

private:
    QScopedPointer<Ui::MainWindow> ui_;
    /*Path to dictionary files*/
    QString filename;
    /*List of words*/
    QStringList Word_list;
    nuspell::Dictionary dict;

    std::string dict_path;
    int cur_pos;

    void save_file(QString name);
};
#endif // MAINWINDOW_HPP
