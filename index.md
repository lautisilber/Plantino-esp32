---
layout: default
title: Labino ESP32
---

## Descripción
Plantino permite controlar diferentes tipos de dispositivos como válvulas y bombas de agua a partir de mediciones de sensores. El enfoque *open-source* del proyecto permite que sea completamente modular. Esto significa que la elección del tipo y cantidad de sensores depende única y exclusivamente de las necesidades de cada situación. Asimismo, los instrumentos controlados no están limitados a bombas de agua y válvulas sino también a cualquier dispositivo que se controle mediante señales de encendido y apagado. Además, los algoritmos que se encargan de controlar dichos dispositivos en base a las mediciones de los sensores son completamente adaptables y se pueden ajustar según la situación lo requiera.

## Características
- Permite ser armado con piezas *open-source* lo que lo hace muy económico y flexible comparado a otras soluciones *closed-source*. Poder incoroprar cualquier sensor o dispositivo estándar y no tener que depender de partes hechas a medida ofrece al usuario acceder a un sistema muy bajo en costos y configurado según sus exigencias.
- Brinda la posibilidad de armar un algoritmo personalizado que responda a las necesidades particulares de cada situación. De esta manera, hasta los requerimientos más exigentes y diversos pueden ser satisfechos.
- Ofrece una interfaz web sencilla e intuitiva para fácilmente poder cambiar la configuración, controlar la integridad de los componentes, y observar en tiempo real las mediciones de los sensores y el estado de los dispositivos.
- Dispone de un servicio integrado (opcional) de recopilación y guardado de datos de los sensores accesible mediante una tarjeta SD, desde la interfaz web y la página [ThingSpeak](https://thingspeak.com/) para analizar las mediciones recopiladas.
- Posee un consumo muy bajo de energía, permitiendo que las exigencias energéticas dependan únicamente de los dispositivos a controlar. Es posible alimentar el circuito (sin contar dispositivos) con una batería.

## Diagrama
![sketch](/assets/Esquema_plantino.png)

## Imágenes
![graphs demo](/assets/Graphs/demo.png)
Gráficos para mostrar los datos recopilados
