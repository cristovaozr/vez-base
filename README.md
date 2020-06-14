# vez-base
V.E.Z. - Volta à estaca zero - código base

Esse projeto é a base para builds de kits pessoais. Guardei-os aqui na experânça que me sejam úteis no futuro e também para outros.

O projeto conta a seguinte estrutura:

```
vez-base/
 +--- arch/
 +--- core/
 |     +--- include/
 |     |     `- device/
 |     +--- src/
 |           `- device/
 +--- freertos/
 |     +--- blinky/
 |     `--- shell/
 +--- tasks/
 |
 `--- ulibc
```

## Pasta "arch"

Possui todas as partes dependente de arquitetura para as plataformas (Bluepill, Waveshare, etc.). Cada pasta em `arch` tem sua própria estrutura e pode ser visto na própria documentação. Para construir uma plataforma para seu próprio projeto basta seguir as instruções em `HOWTO.md`.

## Pasta "core"

Possui pastas `include` e `src` com o código-fonte e cabeçalhos principais. Chamo a atenção às pastas `include/device` e `src/device` que possuem a API de acesso aos dispositivos.

### Pasta "device"

Essa pasta possui a API de acesso aos dispositivos. Há um arquivo `README.md` contendo a explicação geral de como funciona a API de dispositivos.

## Pasta "freertos"

Possui a parte geral do FreeRTOS. Essa pasta **não contém** a parte dependente de arquitetura do FreeRTOS (normalmente os arquivos que se encontram em `portable`).

## Pasta "tasks"

Possui os códigos-fonte das tasks que executam no FreeRTOS.

## Pasta "ulibc"

Possui os códigos-fonte para uma biblioteca C para uso em projetos de sistemas embarcados. Eu tive muita dificuldade com funções `printf()` que chegavam a causar travamentos no microcontrolador. Enquanto eu não descobrir como usar essas funções elas estão implementadas aqui.

O projeto depende da `newlib` e faz link contra a `newlib-nano` para funções que não estão definidas aqui. Há também um sistema de logging pertencente em `log.h` e `log.c`.