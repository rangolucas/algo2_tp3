#include "Table.h"

Table::Table(linear_set<NombreCampo> c, NombreCampo clave){
    _campos=c;
    _campoClave=clave;
}

Table::Table(const Table& aCopiar){
    _campos=aCopiar._campos;
    _campoClave=aCopiar._campoClave;
    _registros=aCopiar._registros;
    _claves=aCopiar._claves;
}

void Table::agregarRegistro(Registro r){
    _registros.insert(r);
    for(NombreCampo i : r.campos()){
        _claves.insert(make_pair(r[i],r));
    }
}

string_map<Registro> Table::clavesARegistros(){
    return _claves;
}

void Table::borrarRegistro(Valor v){
    linear_set<Registro>::iterator it = _registros.find(_claves.at(v));

    if(it != _registros.end()){
        _registros.erase(it);
    }
    /*
    for(NombreCampo i: _registros){
        if(_registros[i]==v){
            _registros.erase(i);
        }
    }*/
    _claves.erase(v);
}

linear_set<NombreCampo> Table::campos(){
    return _campos;
}

NombreCampo Table::campoClave(){
    return _campoClave;
}

linear_set<Registro> Table::registros(){
    return _registros;
}
