# Multiconsult-ppcc
Programa Procesador de Curvas de Carga, Multiconsult Nicaragua.


## Descripci�n
Este software procesa curvas de carga que se obtienen de mediciones del consumo de energ�a en distintos sitios (casas, edificios, instituciones p�blicas, etc.). El programa permite importar archivos de las mediciones en formato Excel y clasificarlas en sectores y subcategor�as (por ejemplo Comercial: Hoteles, Residencial: 101 - 150 kWh/mes, etc.). Las mediciones se pueden visualizar en diagramas, lo cual resulta en curvas de carga, y el programa puede realizar diferentes c�lculos, como por ejemplo calcular el promedio de todas las mediciones de una categor�a, de una fecha o de un d�a de semana especificado. Los datos obtenidos se pueden exportar a un archivo Excel o como imagen en formato PNG.

El programa est� siendo desarrollado en C++11 con QtCreator 6.0.1, utilizando la versi�n 5.15.2 de Qt (licencia GPL v3) y la biblioteca QXlsx (licencia MIT) para leer y escribir archivos Excel. La plataforma prevista y en la cual se realizaron las pruebas es Windows 10.

Repository de GitHub:
https://github.com/ErnestoVivas/Multiconsult-ppcc


## Formato de las Mediciones
Los archivos Excel que contienen las mediciones y que se importan en PPCC deben cumplir con el siguiente formato:

Fecha       | Hora      | kW  
01/01/2001  | 00:00:00  | 100.0  
01/01/2001  | 00:15:00  | 150.6  
01/01/2001  | 00:30:00  | 120.0  
01/01/2001  | 00:45:00  | 110.5  
01/01/2001  | 01:00:00  | 100.5  
    .            .         .  
    .            .         .  
    .            .         .  
02/01/2001  | 00:00:00  | 104.2  
    .            .         .  
    .            .         .  
    .            .         .  

La fecha debe estar en el formato dd/mm/aaaa, la hora debe estar en formato de 24 horas. Las mediciones tienen que tener una frequencia de 15 min o 1 hora. Si los archivos no est�n en este formato PPCC no podr� analizar los datos correctamente y el programa podr�a dejar de funcionar. PPCC no verifica las unidades y asume que todos los valores tienen como unidad kW. Los nombres de las columnas (fecha, hora, kW) pueden ser variables: Ser�n utilizadas para nombrar a los ejes de los diagramas, pero esto se puede cambiar a la hora de generar los diagramas.


## Instalaci�n y ejecuci�n
Existen dos maneras para instalar y ejecutar PPCC:

### Archivo ejecutable
Si cuenta con la carpeta que contiene el archivo ejecutable y las librer�as correspondientes solo tendr� que ejecutar el archivo ppcc.exe (puede copiar la carpeta a cualquier ubicaci�n en el sistema).

### Compilar c�digo fuente
Si cuenta con el c�digo fuente deber� instalar primero Qt5 (PPCC usa Qt 5.15.2, pero la mayor�a de las versiones de Qt5 deber�an funcionar). Abra el proyecto en QtCreator y ejecute run, esto compilar� y ejecutar� el programa. Si desea ejecutar PPCC desde fuera de QtCreator, deber� compilar el programa en QtCreator y luego localizar el archivo ppcc.exe en la carpeta 'build-...'. Para ejecutar ppcc.exe directamente deber� copiar las siguientes bibliotecas a la misma carpeta en la que se encuentra ppcc.exe:
- libgcc_s_seh-1.dll
- libstdc++-6.dll
- libwinpthread-1.dll
- Qt5Charts.dll
- Qt5Core.dll
- Qt5Gui.dll
- Qt5Widgets.dll
- platforms/
  - qdirect2.dll
  - qminimal.dll
  - qoffscreen.dll
  - qwindows.dll

Estas bibliotecas son parte de Qt5 o de MinGW, los archivos se pueden localizar utilizando la funci�n de b�squeda de archivos en el explorador de archivos de Windows.
