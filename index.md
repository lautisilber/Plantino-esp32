---
layout: default
title: Labino ESP32
---

## Descripción
Plantino permite el control de válvulas y bombas de agua a partir de mediciones de sensores. El enfoque *open-source* del proyecto le permite ser completamente modular. Esto significa que la elección del tipo y cantidad de sensores depende sola y únicamente de las necesidades de cada situación. A su vez, los instrumentos controlados no están limitados a bombas de agua y válvulas sino a cualquier dispositivo que se controle mediante señales de encendido y apagado. Además, los algoritmos que se encargan de controlar los dispositivos en base a las mediciones de los sensores son completamente adaptables y se pueden ajustar según la situación lo requiera.

## Características
- El proyecto está armado con piezas *open-source* lo que lo hace muy barato y flexible comparado a otras soluciones *closed-source*. Poder incoroprar cualquier sensor o dispositivo disponible sin tener que usar componentes patentados por empresas (**esto esta muy mal explicado**) permite al usuario hacerse de un sistema extremadamente bajo en costos y hecho a su medida.
- Brinda la posibilidad de armar un algoritmo personalizado que responda a las necesidades particulares de cada situación. De esta manera, hasta los requerimientos más exigentes y exóticos pueden ser satisfechos.
- Ofrece una interfaz web sencilla e intuitiva para fácilmente poder cambiar la configuración, controlar la integridad de los componentes, y observar en tiempo real las mediciones de los sensores y el estado de los dispositivos.
- Tiene integrado un servicio (opcional) de recopilación y guardado de mediciones de los sensores accesible mediante una tarjeta SD, desde la interfaz web y la página [ThingSpeak](https://thingspeak.com/) para analizar los datos recopilados.
- Tiene un consumo extremadamente bajo de energía, permitiendo que las exigencias energéticas dependan virtualmente únicamente de los dispositivos a controlar. Es posible alimentar el circuito (sin contar dispositivos) con una batería.

## Imágenes
![index page](/assets/Esquema_plantino.png)
