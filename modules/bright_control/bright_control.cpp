//=====[Libraries]=============================================================

#include "arm_book_lib.h"

#include "bright_control.h"

#include "light_level_control.h"

//=====[Declaration of private defines]========================================

#define LEDS_QUANTITY 3

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

/* Los pines asociados son PB_4, PA_0, PD_12
 * Para el control de brillo se utiliza un PWM para cada pin
 * Para eso se utiliza un ticker tickerBrightControl que ejecuta tickerCallbackBrightControl cada 1ms
 * Con periodSFloat se configura el periodo del pwm, en este ejemplo en 10ms
 * Se setea el dutty cycle en 0.5
 * onTime y offTime contienen cuantas veces debe haber una interrupcion para cada estado que provoque un cambio en la salida
 */

/*      FUNCION                 FUNCION A LAS QUE LLAMA
 * brightControlInit            setPeriod, setDutyCycle
 * -> inicializa el ticker, setea los periodos a 10ms y los dutty a 0.5 
 *
 * tickerCallbackBrightControl  -
 * -> para cada led cambia el estado de la salida si ya paso el tiempo suficiente
 *
 * setDutyCycle                 -
 * -> setea los valores de onTime y offTime 
 *
 * setPeriod                    -
 * -> setea el periodo del PWM
 */

// se usa un objeto de la clase Ticker para el manejo de interrupciones

DigitalOut RGBLed[] = {(PB_4), (PA_0), (PD_12)};

Ticker tickerBrightControl;

//=====[Declaration and initialization of private global objects]===============

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

//=====[Declaration and initialization of private global variables]============

static int onTime[LEDS_QUANTITY];
static int offTime[LEDS_QUANTITY];

static int tickRateMSBrightControl = 1;
static int tickCounter[LEDS_QUANTITY];

static float periodSFloat[LEDS_QUANTITY];

//=====[Declarations (prototypes) of private functions]========================

static void setPeriod( lightSystem_t light, float period );
static void tickerCallbackBrightControl( );

//=====[Implementations of public functions]===================================

void brightControlInit()
{
    tickerBrightControl.attach( tickerCallbackBrightControl, 
                              ( (float) tickRateMSBrightControl) / 1000.0 );

    setPeriod( RGB_LED_RED, 0.01f );
    setPeriod( RGB_LED_GREEN, 0.01f );
    setPeriod( RGB_LED_BLUE, 0.01f );

    setDutyCycle( RGB_LED_RED, 0.5f );
    setDutyCycle( RGB_LED_GREEN, 0.5f );
    setDutyCycle( RGB_LED_BLUE, 0.5f );
}

void setDutyCycle( lightSystem_t light, float dutyCycle )
{
    onTime[light] = int ( ( periodSFloat[light] * dutyCycle ) * 1000 );
    offTime[light] = int ( periodSFloat[light] * 1000) - onTime[light];
}

//=====[Implementations of private functions]==================================

static void setPeriod( lightSystem_t light, float period )
{
    periodSFloat[light] = period;
}

static void tickerCallbackBrightControl( )
{
    int i;

    for (i = 0 ; i < LEDS_QUANTITY ; i++) {
        tickCounter[i]++;
        if ( RGBLed[i].read() == ON ) {
            if( tickCounter[i] > onTime[i] ) {
                tickCounter[i] = 1;
                if ( offTime[i] ) RGBLed[i] = OFF;
                
            }
        } else {
            if( tickCounter[i] > offTime[i] ) { 
                tickCounter[i] = 1;
                if ( onTime[i] ) RGBLed[i] = ON;
            }
        }
    }
}