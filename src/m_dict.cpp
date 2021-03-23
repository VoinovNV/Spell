#include "m_dict.hpp"
M_dict::M_dict(){
    vec.reserve(1);
}
void M_dict::load(QStringList paths)
{
    if(vec.size()) vec.clear();
    for(int i=0;i<paths.length();i++)
        vec.push_back(nuspell::Dictionary::load_from_path(paths[i].toStdString()));
}
bool M_dict::spell(std::string_view word){
    for(std::size_t i=0;i<vec.size();i++)
        if(vec[i].spell(word)) return true;
    return false;
}
void M_dict::suggest(std::string_view word, std::vector<std::string>& sugs){
    for(std::size_t i=0;i<vec.size();i++){
        std::vector<std::string> sugs_={};
        vec[i].suggest(word,sugs_);
        sugs.insert(sugs.end(),std::make_move_iterator(sugs_.begin()),std::make_move_iterator(sugs_.end()));
    }
    std::sort(sugs.begin(),sugs.end());
    sugs.resize(std::unique(sugs.begin(),sugs.end())-sugs.begin());
}
