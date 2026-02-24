# Administrador de Presupuesto

Una aplicación de escritorio para gestionar el presupuesto personal con seguimiento de ingresos y gastos.

## Requisitos Previos

Antes de ejecutar el proyecto, necesitas tener instalado lo siguiente en tu sistema Windows:

1. Compilador MinGW-w64 GCC
   - Debe estar instalado en: C:\mingw64\
   - Descargar desde: https://www.msys2.org/

2. Git para Windows (opcional, solo si vas a clonar desde un repositorio)

## Estructura del Proyecto

- src/ - Archivos de código fuente
- include/ - Archivos de cabecera (Clay UI y Raylib)
- lib/ - Archivos de bibliotecas
- bin/ - Archivo ejecutable compilado y archivos DLL
- fuente/ - Archivos de fuentes
- budget_data.csv - Tu archivo de almacenamiento de datos del presupuesto

## Cómo Compilar

1. Abre PowerShell o Símbolo del Sistema

2. Navega al directorio del proyecto:
   ```
   cd ruta\a\Ana_Transcriptor
   ```
3. Ejecuta las instalaciones de las herramientas/librerias|
   ```
   pacman -S --needed mingw-w64-ucrt-x86_64-toolchain
   pacman -S --needed make cmake ninja
   pacman -S mingw-w64-ucrt-x86_64-raylib

   ```


4. Ejecuta el script de compilación:
   ```
   .\make
   ```

El script de compilación hará lo siguiente:
- Compilará el código fuente
- Enlazará todas las bibliotecas
- Copiará los archivos DLL necesarios a la carpeta bin
- Ejecutará la aplicación automáticamente

## Cómo Ejecutar

Después de compilar, puedes ejecutar la aplicación directamente:

```
.\bin\BudgetManager.exe
```

O simplemente ejecutar el script de compilación de nuevo:

```
.\make run
```

## Características

- Agregar transacciones de ingresos
- Agregar transacciones de gastos
- Ver ingresos totales, gastos totales y saldo neto
- Guardar datos en un archivo CSV
- Cargar datos desde un archivo CSV
- Exportar informe a un archivo de texto
- Interfaz gráfica de usuario interactiva

## Uso

1. Agregar Ingreso:
   - Ingresa el monto en el campo Cantidad
   - Ingresa una descripción
   - Haz clic en el botón azul "+ Agregar Ingreso"

2. Agregar Gasto:
   - Ingresa el monto en el campo Cantidad
   - Ingresa una descripción
   - Haz clic en el botón rojo "- Agregar Gasto"

3. Guardar Datos:
   - Haz clic en el botón "Guardar en CSV" para guardar tus transacciones

4. Cargar Datos:
   - Haz clic en el botón "Cargar desde CSV" para cargar las transacciones guardadas

5. Ver Transacciones:
   - Todas las transacciones se muestran en la lista con tipo, monto, descripción y fecha

## Almacenamiento de Datos

Todos los datos del presupuesto se almacenan en el archivo budget_data.csv en el directorio raíz del proyecto. Este archivo se crea automáticamente cuando guardas tus datos.

## Resolución de Problemas

1. Si la aplicación no inicia:
   - Asegúrate de que MinGW-w64 esté instalado en C:\mingw64\
   - Verifica que todos los archivos DLL estén presentes en la carpeta bin

2. Si obtienes errores de compilación:
   - Verifica que gcc.exe esté en tu PATH o en C:\mingw64\bin\gcc.exe
   - Asegúrate de que todos los archivos requeridos estén presentes en el proyecto

3. Si la ventana aparece en blanco:
   - Verifica que la carpeta fuente contenga el archivo de fuente Roboto-Regular.ttf

## Requisitos del Sistema

- Windows 7 o posterior
- Mínimo 500 MB de RAM
- 50MB de espacio libre en disco
- Resolución de pantalla: 1280x720 o superior

## Créditos

Construido con:
- Raylib - Biblioteca gráfica
- Clay UI - Motor de diseño
- MinGW-w64 GCC - Compilador