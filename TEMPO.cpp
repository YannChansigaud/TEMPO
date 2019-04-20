/*
 * TEMPO.cpp
 * CREATED on TUESDAY 16 AUGUST 2018
 * by CHANSIGAUD YANN
 */


#if (ARDUINO <  100)
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include <inttypes.h>

#include "TEMPO.h"
// ================================================================================ // ==================================================== CONSTRUCTEURS
TEMPO::TEMPO(){                                                                     // par défaut
}                                                                                   // 
TEMPO::TEMPO( unsigned long p, byte mode ){                                         // avec initialisation
    periode = p;                                                                    // mémorisation periode
    setmode( mode );                                                                // mémorisation mode
    etat_tempo = &tempo_ended;                                                      // 
    temps      = &T_now;                                                            // 
    externVARS = false;                                                             // définie les variables comme "INTERNE"
}                                                                                   // 
TEMPO::TEMPO( unsigned long p, byte mode=AUTOLOOP, bool *etat, unsigned long *T  ){ // avec initialisation et variable de temps
    periode = p;                                                                    // mémorisation periode
    setmode( mode );                                                                // mémorisation mode
    etat_tempo = etat;                                                              // mémorise l'adresse de la variable global de l'état de cette tempo
    temps      = T;                                                                 //                                          de la valeur de millis()
    externVARS = true;                                                              // définie les variables comme "EXTERNE" pour ne pas récupérer millis() à chaque fois
}                                                                                   // 
// ================================================================================ // ==================================================== DEFINITION
void TEMPO::vars( bool *etat, unsigned long *T ){                                   // 
    etat_tempo = etat;                                                              // joint l'adresse de l'état de tempo à la variable global
    temps      = T;                                                                 // 
    externVARS = true;                                                              // 
}                                                                                   // 
void TEMPO::set( unsigned long p ){                                                 // 
    periode = p;                                                                    // on mémorise la période
}                                                                                   // 
void TEMPO::setmode( byte id_mode ){                                                // défini si le TEMPO reboucle en permanence ou pas
    if( id_mode == AUTOLOOP ){    autoLoop = true;            }                     // 
    else{                         autoLoop = false;           }                     // 
}                                                                                   // 
// ================================================================================ // ==================================================== CONTROL
bool TEMPO::ack(){               /*ready*/                                          // 
    if( !externVARS ){           *temps = millis();                             }   // Si on n'utilise pas de variable externe : on va chercher la valeur de millis()
    if( *temps < T_prev ){       T_pass = *temps - 0 + ( MAX_ULONG - T_prev );  }   // ça veux dire que ULONG est repassé à zéro, on mémorise la distance à zéro et la distance à fin
    else{                        T_pass = *temps - T_prev;                      }   // sinon, on mémorise simplement le temps passé
    if( T_pass > periode ){                                                         // Si le temps passé est supérieur à la période
        *etat_tempo = true;                                                         //   on indique que le temps est écoulé
		if( autoLoop == true ){  T_prev = *temps;                               }   //   Si autoLoop alors on enregistre "maintenant"
        return( true );                                                             //   on renvoi vrai
    }                                                                               // 
    else{                                            *etat_tempo = false;           // Si le temps passé est inférieur à la période
        return( false );                                                            //   on renvoi faux
    }                                                                               // 
}                                                                                   // 
void TEMPO::restart(){                                                              // On relance une nouvelle tempo
    T_prev = *temps;                                                                //   On mémorise le temps au démarrage de cette tempo
    *etat_tempo = false;                                                            //   On redéfini la tempo sur false = en cours
}                                                                                   // 
