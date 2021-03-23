#ifndef DICT_SETTINGS_HPP
#define DICT_SETTINGS_HPP

#include <QDialog>
#include "./ui_dict_settings.h"
namespace Ui {
class Dict_settings;
}

class Dict_settings : public QDialog
{
    Q_OBJECT

public:
    explicit Dict_settings(QWidget *parent = nullptr);
    ~Dict_settings();
    QStringList* paths_;
    void update_list();

private slots:
    void on_RemovePathButton_clicked();
    void on_ChButton();
    void on_AddPathButton_clicked();

private:
    QScopedPointer<Ui::Dict_settings> ui_;
};

#endif // DICT_SETTINGS_H
