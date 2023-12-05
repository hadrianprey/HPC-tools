# Lab AI - HPC Tools

### Autores
  - Hadrián Peleteiro Rey 
  - Javier Darriba Gonzalez

## Descripcion
Este repositorio contiene la implementación y los resultados obtenidos en la tarea relacionada con la aceleración del entrenamiento de un modelo de Deep Learning en PyTorch.

### Configuracion del Entorno
- Entorno: `mytorchdist`
- Hardware utilizado:
  - **BASELINE**: 1 Nvidia A100 GPU
  - **Implementacion Distribuida**: 2 nodos con 2 Nvidia A100 GPUs cada uno

## Implementacion BASELINE

Para la evaluación inicial (BASELINE), se seleccionó el modelo de Ejemplo de MNIST usando RNN.
Las redes neuronales recurrentes (RNN) son una clase de redes neuronales bastante potentes para modelar datos de secuencia,
como series temporales o lenguaje natural.
Esquemáticamente, una capa RNN utiliza un bucle for para iterar sobre los pasos de tiempo de una secuencia,
al mismo tiempo que mantiene un estado interno que codifica información sobre los pasos de tiempo que ha observado hasta el momento.
El tiempo total de ejecución del modelo con una única GPU fue de 2 minutos 31 segundos.

- **Tiempo real**: 2 minutos y 30.513 segundos
- **Tiempo de usuario**: 2 minutos y 42.647 segundos
- **Tiempo de sistema**: 10.614 segundos

## Implementacion Distribuida
Se utilizó Lightning Fabric debido a su facilidad y rapidez en la implementación, ya que no se necesitan hacer grandes cambios en el código como tal
Como estrategias a utilizar, se probaron  DP (Data Parallel) y DDP (Distributed Data Parallel). DP es una estrategia simple en la que el modelo se
replica en cada dispositivo y cada réplica procesa un lote de datos diferentes. Luego, los gradientes se calculan
localmente en cada réplica y se promedian antes de realizar la actualización de los pesos del modelo.

Por otra parte, en DDP cada nodo tiene su propia copia del modelo y datos, y la comunicación entre nodos se utiliza 
para sincronizar y promediar los gradientes, como  DDP se enfoca en la distribución de la tarea de entrenamiento en un entorno
distribuido, permite el escalado horizontal a través de nodos en un clúster.

### Resultados de DP
El tiempo total obtenido con la estrategia de Data Parallel fue de aproximadamente 2 minutos y 1 segundo mejorando un 20% respecto a la baseline
#### GPU 1
- **Tiempo real**: 1 minuto y 58.544 segundos
- **Tiempo de usuario**: 3 minutos y 10.929 segundos
- **Tiempo de sistema**: 14.443 segundos
#### GPU 2
- **Tiempo real**: 1 minuto y 59.161 segundos
- **Tiempo de usuario**: 3 minutos y 11.451 segundos
- **Tiempo de sistema**: 13.911 segundos
#### GPU 3
- **Tiempo real**: 1 minuto y 59.463 segundos
- **Tiempo de usuario**: 3 minutos y 11.476 segundos
- **Tiempo de sistema**: 13.947 segundos
#### GPU 4
- **Tiempo real**: 2 minutos y 0.819 segundos
- **Tiempo de usuario**: 3 minutos y 13.093 segundos
- **Tiempo de sistema**: 14.198 segundos

### Resultados de DDP
El tiempo total obtenido con la estrategia de Distributed Data Parallel fue de aproximadamente 1 minuto y 7 segundos mejorando un 56% respecto a la baseline

#### GPU 1
- **Tiempo real**: 1 minuto y 1.535 segundos
- **Tiempo de usuario**: 1 minuto y 2.148 segundos
- **Tiempo de sistema**: 19.432 segundos
#### GPU 2
- **Tiempo real**: 1 minuto y 1.528 segundos
- **Tiempo de usuario**: 1 minuto y 5.518 segundos
- **Tiempo de sistema**: 18.153 segundos
#### GPU 3
- **Tiempo real**: 1 minuto y 1.650 segundos
- **Tiempo de usuario**: 49.065 segundos
- **Tiempo de sistema**: 5.894 segundos
#### GPU 4
- **Tiempo real**: 1 minuto y 1.645 segundos
- **Tiempo de usuario**: 1 minuto y 3.058 segundos
- **Tiempo de sistema**: 19.235 segundos

## Análisis y Conclusión
DDP mostró un mejor rendimiento en términos de tiempo de ejecución en comparación con DP para este modelo.
Cabe destacar que al ejecutar el programa con fabric sin especificar ninguna estrategia este también elige la de 
DDP considerando que es la más adecuada.

### Archivos y Estructura del Repositorio
- **baseline_submit.sbatch**: script SLURM que ejecuta baseline_mnist.sh
- **baseline_mnist.sh**: script que establece el entorno y ejecuta el codigo python baseline.py
- **baseline.py**: Codigo de la implementacion BASELINE
- **submit.sbatch**: script SLURM que ejecuta mnistrnn.sh
- **mnistrnn.sh**: script que establece el entorno y ejecuta el codigo python distributed_code.py
- **distributed_code.py**: Codigo de la implementacion distribuida. Para ejecutar las diferentes estrategias, descomentar la creación del objeto Fabric con la estrategia deseada y comentar el resto
- **README.md**: Este archivo con la explicacion detallada
