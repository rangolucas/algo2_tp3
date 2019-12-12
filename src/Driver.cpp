#include <cassert>
#include <cstdlib>
#include <sstream>

#include "Driver.h"

void Driver::crearTabla(NombreTabla tabla, vector<NombreCampo> campos, NombreCampo clave) {
    linear_set<NombreCampo> algo;
    for(int cl=0; cl<campos.size();cl++){//convierte el vector en set
        algo.insert(campos[cl]);
    }
    Table t = Table(algo,clave);
    _base.AgregarTabla(tabla,t);
}

void Driver::insertarRegistro(NombreTabla t, Registro& r) {
    _base.AgregarRegistro(t,r);
}

void Driver::eliminarRegistro(NombreTabla t, Registro& r) {
    _base.EliminarRegistro(t,r);
}

Respuesta Driver::consultar(const Consulta& q) {
    vector<Registro> res;
    linear_set<Registro> cons = _base.EjecutarConsulta(q);
    for(const Registro& cl : cons){//convierte el vector en set
        res.push_back(cl);
    }
    return res;
}

void Driver::leerDataset(string dataset) {
    ifstream is("datos/" + dataset + ".txt");
    
    vector<string> campos;
    _leerLinea(is, campos);
    crearTabla(dataset, campos, campos[0]);

    int linea = 1;
    while (!is.eof()) {
        vector<string> valores;
        if (!_leerLinea(is, valores)) {
            break;
        }
        if (campos.size() != valores.size()) {
            cerr << "Error leyendo dataset: " << dataset << endl;
            cerr << "El registro en la linea " << linea
                 << " tiene " << valores.size() << " campos"
                 << " pero la tabla declara " << campos.size() << " campos."
                 << endl;
            exit(1);
        }
        Registro r;
        for (size_t i = 0; i < campos.size(); i++) {
//            cout << "Se inserto: " << campos[i] << " - " << valores[i] << endl;
            Valor &v = valores[i];
            NombreCampo &c = campos[i];
            r.DefinirCampo(c, v);
        }
        insertarRegistro(dataset, r);
    }
}

bool Driver::_leerLinea(ifstream& is, vector<string>& valores) const {
    string linea;
    if (!getline(is, linea)) {
        return false;
    }

    istringstream isl(linea);
    string valor;
    while (getline(isl, valor, ',')) {
        valores.push_back(valor);
    }
    return true;
}

