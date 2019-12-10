#ifndef TP3_BASEDEDATOS_H
#define TP3_BASEDEDATOS_H
#include <set>
#include <vector>

#include "Registro.h"
#include "Consulta.h"
#include "Tipos.h"
#include "Table.h"
#include "modulos_basicos/string_map.h"
#include "modulos_basicos/linear_set.h"
//#include "modulos_basicos/string_map.hpp"

using namespace std;

class BaseDeDatos{
public:
    BaseDeDatos();
    BaseDeDatos(NombreTabla n, Table t);
    void AgregarTabla(NombreTabla n, Table& t);
    void EliminarTabla(NombreTabla n);
    void AgregarRegistro(NombreTabla n, Registro& r);
    void EliminarRegistro(NombreTabla n, Registro r);
    linear_set<Registro> FROM(NombreTabla n);
    linear_set<Registro> SELECT(const Consulta& q, NombreCampo c, Valor v);
    linear_set<Registro> MATCH(const Consulta& q, NombreCampo c1, NombreCampo c2);
    linear_set<Registro> PROJ(const Consulta& q, linear_set<NombreCampo> cs);
    linear_set<Registro> RENAME(const Consulta& q, NombreCampo c1, NombreCampo c2);
    linear_set<Registro> INTER(const Consulta& q1, const Consulta& q2);
    linear_set<Registro> UNION(const Consulta& q1, const Consulta& q2);
    linear_set<Registro> PRODUCT(const Consulta& q1, const Consulta& q2);
    linear_set<Registro> JOIN(NombreTabla t1, NombreTabla t2);
    linear_set<Registro> EjecutarConsulta(const Consulta& q);
   /* set<Registro> FiltrarPorValor(set<Registro> rgs, NombreCampo c, Valor v);
    set<Registro> FiltrarPorCoincidenciaDeCampos(set<Registro> rgs, NombreCampo c1, NombreCampo c2);
    set<Registro> RenombrarCampo(set<Registro> rgs, NombreCampo c1, NombreCampo c2);
    set<Registro> InterseccionConsulta(set<Registro> rgs1, set<Registro> rgs2);
    set<Registro> UnirConsulta(set<Registro> rgs1, set<Registro> rgs2);
    set<Registro> ProductoConsulta(set<Registro> rgs1, set<Registro> rgs2);*/
private:
    string_map<Table> _tablas;
    string_map<linear_set<NombreCampo>> _campos;
    string_map<NombreCampo> _clave;
    string_map<linear_set<Registro>> _registrosTabla;
    string_map<string_map<string_map<Registro>>> _registroPorValor;
};

//#include "BaseDeDatos.cpp"

#endif //TP3_BASEDEDATOS_H
