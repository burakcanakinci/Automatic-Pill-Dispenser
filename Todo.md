# Dispensador To-Do

Funcionalidades por agregar al dispensador de pastillas.

## Dispensador

- [x] Probar con un vector Pastilla pastillas[4]
- [x] Agregar overload de _dispensarPastilla()_ tomando como parámetro el nombre asignado a la pastilla
- [x] Agregar overload de _agregarPastillas()_ tomando como parámetro el nombre asignado a la pastilla
- [x] Agregar funcion _dispensarPastillas()_ para dispensar múltiples pastillas que sea llamada mediante el RTC
- [x] ~~Agregar funcion _vibrar()_ para que la bandeja vibre y acomode las pastillas~~
- [ ] ~~Agregar detector del número de recipientes en caso de que no estén insertados todos~~
- [ ] ~~Modificar forma de los recipientes para que sean más pequeños~~
- [x] ~~Agregar una rampa o forma cónica para concentrar las pastillas en un solo punto~~
- [ ] ~~Escoger una ventosa de fuelle con una boquilla más grande para una mejor succión~~
- [x] Agregar una válvula electrónica para liberar presión y que caiga la pastilla
- [ ] Agregar LEDs de estado
- [x] Agregar LED que ilumine el vaso cuando una dosis esté lista
- [x] Agregar Buzzer
- [x] Cambiar initializer list para inicializar objetos con parámetros (ej. brazo() -> brazo(pinsBrazo))
- [x] Verificar si pastilla está configurada antes de realizar accion (ADD, SET, DEL, DIS, etc.)
- [x] Agregar un sensor que detecte cuando se retira el vaso de la bandeja conteniendo las pastillas dispensadas
  - [x] Apagar el LED del vaso cuando se retire
  - [x] Seguir notificando cada x tiempo con sonido hasta que se retire el vaso
- [x] Guardar datos en la EEPROM para evitar que se borren
- [ ] Optimizar código con interrupciones
- [x] Configurar número de pasos del brazo para las piezas 3D

## Detector de pastillas

- [x] Agregar un sensor que detecte cuando la pastilla cayó
- [x] Mientras no se active el sensor, que el sistema siga intentando dispensar la pastilla (Máx. x intentos)

## Integración de Pantalla

- [x] Creación de clase **Teclado** con un vector de 6 botones de clase **Boton**
- [x] Aumentar valores en incrementos de 5 para minutos
- [x] Agregar función para agregar pastillas
- [x] Agregar función para eliminar pastillas
- [x] Crear un objeto auxiliar tipo **Pastilla** y enviarlo a un _copy constructor_
- [x] Arreglar error que corrompe datos al terminar la configuración de las pastillas
  - Solución: Faltaba el constructor y destructor de la clase **Reloj**
- [x] Agregar un índice de página para imprimir la pantalla
- [x] Arreglar error baja el voltaje de la pantalla al dispensar pastillas
  - Solución: La válvula electrónica se quedaba HIGH cuando debía bajar a LOW
- [ ] ~~Crear un arreglo de punteros de funciones para ciclar las páginas a mostrar~~

## Integración de módulo RTC

- [x] Agregar funcionalidad de hora y día a la clase _Pastilla_
- [x] Cuando sea la hora especificada, llamar la función _dispensarPastilla()_
- [ ] Dejar de dispensar cuando se haya cumplido el tratamiento (cf. Duración de tratamiento)
- [ ] ~~Agregar interrupciones necesarias para no estar haciendo polling~~
- [x] Configurar la hora del RTC mediante la app de android

## Integración de módulo Wi-Fi + Bluetooth

- [x] Lograr comunicación via bluetooth con la aplicación Android
- [x] Crear función que lea el opcode de la cadena de texto enviada por BT y que ejecute la función correspondiente (Ej. DEL = _eliminarPastilla()_, SET = _setPastilla()_)

## Aplicación Android

- [ ] Sincronizar app Android con cambios a pastillas modificadas localmente
- [ ] Sincronizar automáticamente la hora al conectarse por BT
- [x] Agregar conexión a BT en pantalla **Programar**
- [x] En la pantalla de **Programar** agregar botones para:
  - [x] Dispensar pastilla
  - [ ] ~~Eliminar pastilla~~
- [ ] Notificar cuando la pastilla caduque
