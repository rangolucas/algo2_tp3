#include "BaseDeDatos.h"

BaseDeDatos::BaseDeDatos(){}

BaseDeDatos::BaseDeDatos(NombreTabla n, Table t){
    _tablas.insert(make_pair(n,t));
    _campos.insert(make_pair(n,t.campos()));
    _clave.insert(make_pair(n,t.campoClave()));
    _registrosTabla.insert(make_pair(n,t.registros()));
    string_map<string_map<Registro>> nombre_campo;
    nombre_campo.insert(make_pair(t.campoClave(),t.clavesARegistros()));
    string_map<string_map<string_map<Registro>>> nombre_tabla;
    nombre_tabla.insert(make_pair(n,nombre_campo));

    _registroPorValor =nombre_tabla;
}

void BaseDeDatos::AgregarTabla(NombreTabla n, Table& t){
    _tablas.insert(make_pair(n,t));
    _registrosTabla.insert(make_pair(n,linear_set<Registro>()));
    _campos.insert(make_pair(n,t.campos()));
    _clave.insert(make_pair(n,t.campoClave()));
    string_map<string_map<Registro>> res;
//    for(linear_set<NombreCampo>::iterator it= t.campos().begin(); it!=t.campos().end(); ++it){
    for(NombreCampo campo : t.campos()) {
        string_map<Registro> r;
        res.insert(make_pair(campo, r));
    }
    _registroPorValor.insert(make_pair(n,res));
}

void BaseDeDatos::EliminarTabla(NombreTabla n){
    _tablas.erase(n);
    _registrosTabla.erase(n);
}

void BaseDeDatos::AgregarRegistro(NombreTabla n, Registro& r){
    Table t = _tablas.at(n);
    _registrosTabla.at(n).insert(r);
    _tablas.at(n).agregarRegistro(r);
    auto map = _registroPorValor.at(n);
    const Valor &valor = r[t.campoClave()];
    map.at(t.campoClave()).insert(make_pair(valor, r));
}

void BaseDeDatos::EliminarRegistro(NombreTabla n, Registro r){
    Table t = _tablas.at(n);
    (_registrosTabla.at(n)).erase(r);
    _tablas.at(n).borrarRegistro(r[t.campoClave()]);
    (_registroPorValor.at(n)).at(t.campoClave()).erase(r[t.campoClave()]);
}

linear_set<Registro> BaseDeDatos::FROM(NombreTabla n){
    const linear_set<Registro> registros = _registrosTabla.at(n);
    return registros;
}

linear_set<Registro> BaseDeDatos::SELECT(const Consulta& q, NombreCampo c, Valor v){
    linear_set<Registro> res;
    linear_set<Registro> rgs = EjecutarConsulta(q);
    for(Registro i :rgs){
        if(i[c]==v){
            res.insert(i);
        }
    }
    return res;
}

linear_set<Registro> BaseDeDatos::MATCH(const Consulta& q, NombreCampo c1, NombreCampo c2){
    linear_set<Registro> res;
    linear_set<Registro> rs = EjecutarConsulta(q);
    for(Registro i: rs){
        if(i[c1]==i[c2]){
            res.insert(i);
        }
    }
    return res;
}

linear_set<Registro> BaseDeDatos::PROJ(const Consulta& q, linear_set<NombreCampo> cs){
    linear_set<Registro> res;
    linear_set<Registro> regs = EjecutarConsulta(q);
    for(const Registro& i: regs){
        linear_set<NombreCampo> camp = i.campos();
        Registro nuevoReg = Registro();
        for(const NombreCampo& j: camp){
            if(cs.count(j)){
                nuevoReg.DefinirCampo(j, i.at(j));
            }
        }
        res.insert(nuevoReg);
    }
    return res;
}

linear_set<Registro> BaseDeDatos::RENAME(const Consulta& q, NombreCampo campoViejo, NombreCampo campoNuevo){
    linear_set<Registro> res;
    linear_set<Registro> registros = EjecutarConsulta(q);
    for(Registro registro: registros){
        Registro nuevoReg = Registro();
        linear_set<NombreCampo> campos = registro.campos();
        for(NombreCampo campo: campos){
            if(campoViejo == campo){
                nuevoReg.DefinirCampo(campoNuevo, registro[campo]);
            } else {
                nuevoReg.DefinirCampo(campo, registro[campo]);
            }
        }
        res.insert(nuevoReg);
    }
    return res;
}

linear_set<Registro> BaseDeDatos::INTER(const Consulta& q1, const Consulta& q2) {
    linear_set<Registro> res;
    linear_set<Registro> regsC1 = EjecutarConsulta(q1);
    linear_set<Registro> regsC2 = EjecutarConsulta(q2);
    for(Registro i: regsC1){
        for(Registro j: regsC2){
            if(i==j)
                res.insert(i);
        }
    }
    return res;
}

linear_set<Registro> BaseDeDatos::UNION(const Consulta& q1, const Consulta& q2){
    linear_set<Registro> res = EjecutarConsulta(q1);
    linear_set<Registro> regsC2 = EjecutarConsulta(q2);
    for(Registro i: regsC2){
        res.insert(i);
    }
    return res;
}

linear_set<Registro> BaseDeDatos::PRODUCT(const Consulta& q1, const Consulta& q2){
    linear_set<Registro> res;
    linear_set<Registro> regsC1 = EjecutarConsulta(q1);
    linear_set<Registro> regsC2 = EjecutarConsulta(q2);
    for(Registro i: regsC1){
        for(Registro j: regsC2){
            Registro nr;
            linear_set<NombreCampo> campC1 = i.campos();
            for(NombreCampo d: campC1){
                nr.DefinirCampo(d,i[d]);
            }
            linear_set<NombreCampo> campC2 = j.campos();
            for(NombreCampo e: campC2){
                nr.DefinirCampo(e,j[e]);
            }
            res.insert(nr);
        }
    }
    return res;
}

linear_set<Registro> BaseDeDatos::JOIN(NombreTabla t1, NombreTabla t2){
    linear_set<Registro> res;
    NombreTabla max;
    NombreTabla min;
    if(FROM(t1).size()>FROM(t2).size()){
        max= t1;
        min= t2;
    }
    else{
        max=t2;
        min=t1;
    }
    Table tablaMin =_tablas.at(min);
    Table tablaMax =_tablas.at(max);
    string_map<Registro> clavesARegistrosMin = tablaMin.clavesARegistros();
    string_map<Registro> clavesARegistrosMax = tablaMax.clavesARegistros();
    vector<string> clavesDeMin = clavesARegistrosMin.claves();
    for(int cl=0; cl<clavesDeMin.size();cl++){
        Registro r = clavesARegistrosMin.at(clavesDeMin[cl]);
        Registro rprima = clavesARegistrosMax.at(clavesDeMin[cl]);
        Registro nr;
        linear_set<NombreCampo> camposDeR = r.campos();
        linear_set<NombreCampo> camposDeRprima = rprima.campos();
        for(NombreCampo c : camposDeR){
            nr.DefinirCampo(c,r[c]);
        }
        for(NombreCampo cprima : camposDeRprima){
            nr.DefinirCampo(cprima,rprima[cprima]);
        }
        res.insert(nr);
    }
    return res;
}

linear_set<Registro> BaseDeDatos::EjecutarConsulta(const Consulta& q){
    linear_set<Registro> res;
    TipoConsulta i=q.tipo_consulta();
    switch (i){
        case TipoConsulta::FROM  : {
            NombreTabla nombreTabla = q.nombre_tabla();
            res = FROM(nombreTabla);
        }
            break;
        case TipoConsulta::SELECT :
            res = SELECT(q.subconsulta1(), q.campo1(), q.valor());
            break;
        case TipoConsulta::MATCH :
            res= MATCH(q.subconsulta1(), q.campo1(), q.campo2());
            break;
        case TipoConsulta::PROJ :
            res= PROJ(q.subconsulta1(), q.conj_campos());
            break;
        case TipoConsulta::RENAME :
            res= RENAME(q.subconsulta1(), q.campo1(), q.campo2());
            break;
        case TipoConsulta::INTER :
            res= INTER(q.subconsulta1(), q.subconsulta2());
            break;
        case TipoConsulta::UNION :
            res= UNION(q.subconsulta1(), q.subconsulta2());
            break;
        case TipoConsulta::PRODUCT :
            res= PRODUCT(q.subconsulta1(), q.subconsulta2());
            break;
    }
    return res;
}
/*
linear_set<Registro> BaseDeDatos::FiltrarPorValor(linear_set<Registro> rgs, NombreCampo c, Valor v){

}

linear_set<Registro> BaseDeDatos::FiltrarPorCoincidenciaDeCampos(linear_set<Registro> rgs, NombreCampo c1, NombreCampo c2){

}

linear_set<Registro> BaseDeDatos::RenombrarCampo(linear_set<Registro> rgs, NombreCampo c1, NombreCampo c2){

}

linear_set<Registro> BaseDeDatos::InterseccionConsulta(linear_set<Registro> rgs1, linear_set<Registro> rgs2){

}

linear_set<Registro> BaseDeDatos::UnirConsulta(linear_set<Registro> rgs1, linear_set<Registro> rgs2){

}

linear_set<Registro> BaseDeDatos::ProductoConsulta(linear_set<Registro> rgs1, linear_set<Registro> rgs2){

}*/