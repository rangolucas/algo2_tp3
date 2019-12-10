#include "Registro.h"

using namespace std;

Registro::Registro() {}

Registro::Registro(const Registro& aCopiar){
    _reg=aCopiar._reg;
    _campos=aCopiar._campos;
}

Registro::Registro(NombreCampo c, Valor v) {
    _reg.insert(make_pair(c,v));
    _campos.insert(c);
}

void Registro::DefinirCampo(NombreCampo c, const Valor v){
    _reg.insert(make_pair(c,v));
    _campos.insert(c);
}

linear_set<NombreCampo> Registro::campos() const {
    return _campos;
}

bool Registro::operator==(const Registro& r) const{
    return _campos == r.campos() && _reg == r._reg;
}

Valor Registro::operator[](const NombreCampo& campo) {
    return _reg.at(campo);
}

Valor Registro::at(const NombreCampo& campo) const {
    return _reg.at(campo);
}

