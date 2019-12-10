#ifndef __REGISTRO_H__
#define __REGISTRO_H__

#include <set>
#include <vector>

#include "Tipos.h"
#include "modulos_basicos/string_map.h"
#include "modulos_basicos/linear_set.h"
using namespace std;

class Registro {
public:
    Registro();
    Registro(const Registro& aCopiar);
    Registro(NombreCampo c, Valor v);
    linear_set<NombreCampo> campos() const;
    void DefinirCampo(NombreCampo c, Valor v);
    bool operator==(const Registro& r) const;
    Valor operator[](const NombreCampo& campo);
    Valor at(const NombreCampo& campo) const;
private:
    string_map<Valor> _reg;
    linear_set<NombreCampo> _campos;
};

#endif /*__REGISTRO_H__*/
