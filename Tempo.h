/*
 TEMPO.h
 CREATED on TUESDAY 16 AUGUST 2018
 by CHANSIGAUD YANN

La classe utilise des variables UNSIGNED LONG ( 4 bits ). Si la classe dois être instanciée plusieurs fois pour avoir plusieurs timer,
utilisez le constructeur dédié : TEMPO( unsigned long, byte, bool*, unsigned long* ) afin d'utiliser des variables globales externes.
Ces variables externes devront être déclarées et initialisées avant l'instanciation des objects de cette classe. Leurs adresse respectives
seront transmises en paramètre dans le constructeur à chaque object de cette classe afin d'optimiser la RAM. 

> unsigned long *T :
pointeur vers la variable globale du temps millis().
Afin d'optimiser temps d'exécution et la RAM, il convient de n'y stocker la valeur de millis() qu'une seule fois par boucle loop.

> bool *etat :
pointeur vers la variable globale d'état de la tempo.
Il faut une variable d'état par temporisateur. La fonction ack() peut être appelée en début de fonction loop et/ou à n'importe quel moment.
La variable *etat sera à TRUE si le temps est écoulé
Cette variable d'état pourra être utilisée plusieurs fois dans le code sans pour autant refaire la comparaison vis-à-vis du temps écoulé (optimisation du temps d'execution)

> bool ack() :
Fonction utile pour les timers type métronome afin d'actualiser un affichage 10 fois par seconde ou une mesure que toute les 5 secondes
Si le autoLoop = TRUE, relance une nouvelle temporisation
Attention *etat restera à TRUE QUE lorsque ack() sera vrai, à l'appel de ack() suivant, *etat repassera à FALSE.
TRUE  = temps écoulé         >>>  bool *etat sera passé à TRUE
FALSE = temps non écoulé     >>>  bool *etat sera passé à FALSE

> void restart() :
Fonction utile pour les timers type temporisation : permet d'avoir un délai d'attente sans fonction bloquante (type while ou delai).
En mode TIMEOUT  permet de relancer la tempo à n'importe quel moment, même si elle est déjà en cours
En mode AUTOLOOP permet de réinitialiser à zéro la tempo même avant qu'elle n'arrive à sa fin

Pour une optimisation accrue : mettre en commentaire les fonctions et variables inutilisées surtout en variable externe
*/
#ifndef TEMPO_h
#define TEMPO_h
#include <inttypes.h>

#define AUTOLOOP 1
#define TIMEOUT  2
#define MAX_ULONG 4294967295

class TEMPO{
  public :
    /*!
    @method     
    @abstract   Class constructor. 
    @discussion Constructeur par défaut
    @param      aucun
    */
    TEMPO();
    /*!
    @method     
    @abstract   Class constructor . 
    @discussion Constructeur pour utilisation simple (recommendé pour 2 ou 3 timers)
    @param      p [unsigned long] periode du timer
	@param      mode [byte] AUTOLOOP façon métronome ou TIMEOUT façon temporisation
    */
    TEMPO( unsigned long p, byte mode=AUTOLOOP  );
    /*!
    @method     
    @abstract   Class constructor . 
    @discussion Constructeur pour utilisation multiple (recommendé pour x timers)
    @param      p [unsigned long] periode du timer
	@param      mode [byte] AUTOLOOP (1) façon métronome ou TIMEOUT (2) façon temporisation
	@param     *etat [bool] adresse de variable externe à la classe permettant d'avoir l'état de la tempo TRUE= tempo terminée
	@param     *T [unsigned long] adresse de variable externe à la classe permettant de stocker la valeur millis() qu'une seule fois par loop (optimisation du temps d'exécution)
    */
	TEMPO( unsigned long p, byte mode=AUTOLOOP, bool *etat=NULL, unsigned long *T=NULL  );
    /*!
    @method     
    @abstract   Fonction de définition . 
    @discussion Fonction de transmission des adresses des variables externes à la classe (optimisation RAM)
    @param     *etat [bool] adresse de variable externe à la classe permettant d'avoir l'état de la tempo TRUE= tempo terminée
	@param     *T [unsigned long] adresse de variable externe à la classe permettant de stocker la valeur millis() qu'une seule fois par loop (optimisation du temps d'exécution)
    */
	void vars( bool *etat, unsigned long *T );
    /*!
    @method     
    @abstract   Fonction de définition . 
    @discussion Fonction de définition de la valeur de la période en milliseconde
    @param      p [unsigned long] periode du timer
	*/
	void set( unsigned long p );
    /*!
    @method     
    @abstract   Fonction de définition . 
    @discussion Fonction de définition du mode de fonctionnement 
    @param      id_mode [byte] 1==AUTOLOOP 2==TIMEOUT
	*/
	void setmode( byte id_mode );
    /*!
    @method     
    @abstract   Fonction de control . 
    @discussion Fonction de control du timer. Si mode=AUTOLOOP alors le timer et relancé directement
    @param      aucun
	@return     TRUE = temps écoulé. FALSE = temps non écoulé.
	*/
	bool ack();
    /*!
    @method     
    @abstract   Fonction de control . 
    @discussion Fonction pour relancer le timer. Utile dans le mode TIMEOUT pour relancer une temporisation. Si AUTOLOOP, relance la tempo à zéro
    @param      aucun
	*/
	void restart();


  private :

    /*!
    @variable     
    @abstract   variable d'état
    @discussion enregistre le mode de fonctionement AUTOLOOP ou TIMEOUT
    @valeur     AUTOLOOP -> true, TIMEOUT -> false
	*/
	bool autoLoop    = false;
	/*!
    @variable     
    @abstract   variable d'état
    @discussion enregistre l'état des variables EXTERNE ou LOCALE
    @valeur     EXTERNE -> true, LOCALE -> false
	*/
	bool externVARS  = false;
    /*!
    @variable     
    @abstract   variable d'état
    @discussion enregistre l'état de la TEMPO. Utile en mode TIMEOUT afin de ne pas appeler millis() à chaque control avec ack()
	*/
    bool tempo_ended = false;
	/*!
    @pointeur     
    @abstract   variable d'état
    @discussion enregistre l'adresse de la variable externe, variable d'état de cette TEMPO
	@valeur     TERMINEE -> true, EN COURS -> false
	*/
	bool *etat_tempo;
	/*!
    @variable     
    @abstract   variable de temps
    @discussion enregistre le temps millis() (exprimé en milliseconde) lors du précédent control. Utile pour détecter lorsque millis() reboucle à zéro
	*/
	unsigned long  T_prev  = 0;
    /*!
    @variable     
    @abstract   variable de temps
    @discussion enregistre le temps maintenant (exprimé en milliseconde)
	*/
	unsigned long  T_now   = 0;
	/*!
    @variable     
    @abstract   variable de temps
    @discussion enregistre le temps passé (exprimé en milliseconde)
	*/
	unsigned long  T_pass  = 0;
    /*!
    @variable     
    @abstract   variable de temps
    @discussion enregistre la période de la tempo (exprimé en milliseconde)
	*/
	unsigned long  periode = 0;
    /*!
    @pointeur     
    @abstract   variable de temps
    @discussion enregistre l'adresse de la variable externe
	*/
	unsigned long  *temps;
    
};

#endif
