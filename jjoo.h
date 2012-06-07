#ifndef JJOO_H
#define JJOO_H

#include "competencia.h"

typedef pair<int, pair<int, int> > infoM;

class JJOO{
    public:

        JJOO() {}
        // |cronograma|==cantDias. Si un d�a no hay competencias,q est� la lista vac�a.
        //JJOO(const int anio, const Lista<Atleta>& atletas, const Lista<Lista<Competencia> >& competenciasPorDia);
        JJOO(const int anio, const Lista<Atleta>& atletas, const Lista<Lista<Competencia> >& competenciasPorDia) {

        }
        int anio() const {
        	return 1;
        }
        Lista<Atleta> atletas() const {
        	Lista<Atleta> lista;
        	return lista;
        }
        int cantDias() const {
        	return 1;
        }
        int jornadaActual() const {
        	return 1;
        }
        Lista<Competencia> cronograma(const int dia) const {
        	Lista<Competencia> lista;
        	return lista;
        }

        Lista<Competencia> competencias() const {
        	Lista<Competencia> lista;
        	return lista;
        }
        Lista<Competencia> competenciasFinalizadasConOroEnPodio() const {
        	Lista<Competencia> lista;
        	return lista;
        }

        Lista<Atleta> dePaseo() const {
        	Lista<Atleta> lista;
        	return lista;
        }
        Lista<pair<Pais,Lista<int> > > medallero() const {
        	Lista<pair<Pais,Lista<int> > > lista;
        	return lista;
        }
        int boicotPorDisciplina(const Categoria cat, const Pais p) {
        	return 1;
        }
        Lista<Atleta> losMasFracasados(const Pais p) const {
        	Lista<Atleta> lista;
        	return lista;
        }
        void liuSong(const Atleta& a, const Pais p) {

        }
        Atleta stevenBradbury() const {
        	Atleta atleta;
        	return atleta;
        }
        bool uyOrdenadoAsiHayUnPatron() const {
        	return true;
        }
        Lista<Pais> sequiaOlimpica() const {
        	Lista<Pais> lista;
        	return lista;
        }
        void transcurrirDia() {

        }

		bool operator==(const JJOO& j) const {
			return true;
		}

        void mostrar(std::ostream& os) const {

        }
		void guardar(std::ostream& os) const {

		}
		void cargar (std::istream& is) {

		}

    private:
        int _anio;
        Lista<Atleta> _atletas;
        int _jornadaActual;
        Lista<Lista<Competencia> > _competenciasPorDia;         // En la i-�sima posici�n de la lista, las competencias del d�a i+1.

        enum {ENCABEZADO_ARCHIVO = 'J'};
};
std::ostream & operator<<(std::ostream & os,const JJOO & j);

#endif // JJOO_H

