#ifndef TP3_TABLE_H
#define TP3_TABLE_H

#include <set>
#include <vector>

#include "Registro.h"
#include "Consulta.h"
#include "Tipos.h"
#include "modulos_basicos/string_map.h"

using namespace std;

class Table {
public:
    Table(linear_set<NombreCampo> c, NombreCampo clave);
    Table(const Table& aCopiar);
    void agregarRegistro(Registro r);
    string_map<Registro> clavesARegistros();
    void borrarRegistro(Valor v);
    linear_set<NombreCampo> campos();
    NombreCampo campoClave();
    linear_set<Registro> registros();
private:
    linear_set<NombreCampo> _campos;
    linear_set<Registro> _registros;
    NombreCampo _campoClave;
    string_map<Registro> _registrosPorClave;
};

//#include "Table.cpp"

#endif //TP3_TABLE_H
