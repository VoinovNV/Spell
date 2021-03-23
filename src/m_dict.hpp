#ifndef M_DICT_HPP
#define M_DICT_HPP
#include <string>
#include <vector>
#include <nuspell/dictionary.hxx>
#include <QStringList>
#include <QString>
class M_dict
{
public:
    M_dict();
    ~M_dict()=default;
    void load(QStringList paths);
    void suggest(std::string_view word, std::vector<std::string>& sugs);
    bool spell(std::string_view word);
private:
    std::vector<nuspell::Dictionary> vec;

};

#endif // M_DICT_HPP
