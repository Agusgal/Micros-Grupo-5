# TODOs

COSAS PARA HACER
    - [x] Ver carga de CPU
    - [x] Manual de usuario
    - [x] Usar Leds
    - [x] Armar presentacion
    - [x] Armar diagrama de arquitectura/drivers/fsm 
    - [x] Agregar animacion, correccion de mensajes de error y otros mensajes
    - [x] Test, mucho test
    - [x] "Emprolijar"
    - [x] Compilar sin warnings

EN TODOS LADOS: 
    - [x] AGREGAR EVENT QUEUE.

En add user:
    - [x] Cambiar nombre a main menu


En id_state.c, pin_state.c y user_input:
    - [x] Resetear el timer de timeout en algunas funciones 
    - [x] En algunas actualizar el display
    - [x] Chequear ID con la base de datos 
    - [x] Tirar eventos a la fsm para cambiar de estados 
    - [x] Hacer checkCardID
    - [x] Completar id_fail() 
    - [x] Shifteo del display
    - [x] Blink del display en el caracter en el que esta el usuario
    - [x] Hacer id_cardFail()
    - [x] Completar id_onIdOk() en id_state
    - [x] Timer del title
    - [x] Mostrar titulo
    - [x] Hacer que funcione el backsplace cuando estas en la ultima posicion
    
En FSM:
    - [x] Inicializar cola de eventos
    - [x] Guardar id y pin en una estructura. (Se guarda en base de datos)
    - [x] Hacer getIdArray y getPinArray al pasar de estados.

En main_menu_state.c
    - [x] Ver como mostrar las cosas en el display 
    - [x] hacer funcion cardSwipe
    - [x] hacer funcion initLogin

En fail_state.c
    - [x] ver bien como definir esas funciones :(

En admin_state.c
    - [x] Agregar eventos al seleccionar una opcion del display
    - [x] Mostrar animacion en initAdminMenu 


En usr_state.c
    - [x] Agregar eventos al seleccionar una opcion del display
    - [x] Mostrar animacion en initUserMenu

En open_state.c
    - [x] Led On al entrar
    - [x] Iniciar timer al entrar
    - [x] Led Off al terminar el timer

En config_me.c
    - [x] Todo

En config_device.c:
    - [x] Mostrar en el display las opciones y un titulo

En DataBase.c
    - [x] Completar removeUser
    - [x] Completar StartResetUserBlocked con el evento de tiempo 

En DataBase.h
    - [x] Completar Comentarios en database.h
    - [x] Completar el comentario faltante en la funcion getIdByCardNumber(int8_t cardNumber[])  

En App.c
    - [x] eventos de timer se agregan solos a cola????    
    - [x] poner el nuevo driver de button y ver si hacer uno generico o preguntar con el boton
    - [x] hacer funcion que llene la cola de eventos, revisando timers, botones, etc 

Branch Led:

    - [x] Mergear ambos branch
