# Escrevendo uma plataforma para seu microcontrolador

Esse documento explica como configurar uma pasta de plataforma (arch) para seu microcontrolador. O projeto [vez-arch-bluepill](https://github.com/cristovaozr/vez-arch-bluepill) é um exemplo de como fazer uma plataforma.

# Realizando a configuração para o Makefile

O `Makefile` principal (que está na raíz do projeto) depende da inclusão de um arquivo `config.mk`. Esse arquivo contem a configuração (símbolos e outras coisas) necessárias para a compilação da plataforma. É necessário definir os seguintes símbolos:

|       Símbolo      | Significado |
|---|---|
| `ARCH_MCU` | Deve definir tudo da CPU conforme flags do GCC: -mcpu, -mthumb, etc. |
| `ARCH_C_DEFS` | Deve conter todos os `#define` de C/C++ (e.g.: `-DCLOCK=8000000`) |
| `ARCH_C_INCLUDES` | Deve conter todos os caminhos de `#include` da plataforma para C/C++ |
| `ARCH_C_SOURCES` | Deve conter todos os arquivos de código-fonte C/C++ para compilação |
| `ARCH_AS_INCLUDES` | Deve conter todos os caminhos de `#include` da plataforma para assembly |
| `ARCH_ASM_SOURCES` | Deve conter todos os arquivos de códig-fonte assembly para compilação |
| `ARCH_LDSCRIPT` | Deve conter o caminho para o arquivo de linker |

# API mínima para compilação

Para compilar uma plataforma os seguintes arquivos devem existir:

| Arquivo | Explicação |
|---|---|
| Inicialização de hardware | Implementa a API definida no arquivo `${VEZ-BASE}/core/include/hw_init.h`. Normalmente as plataformas nomeiam esse arquivo `hw_init.c` |
| Arquivo de implementação de devices | Implementa a API definida no arquivo `${VEZ-BASE}/core/include/device/device.h` e implementa a função `const void * device_get_by_name(const char *dev_name)`

# Hardware mínimo que deve estar disponível

Devem haver os seguintes dispositivos definidos:
* `DEFAULT_USART`
* `DEFAULT_LED`

```c
#include "include/device/device.h"

// Um GPIO (normalmente atribuído a um LED) para a task blinky
const struct gpio_device *led = device_get_by_name(DEFAULT_LED);

// Uma USART para a task shell
const struct usart_device *usart = device_get_by_name(DEFAULT_USART);
```

# Coisas pendentes

As seguintes pendências ainda não estão resolvidas para desacoplamento completo das plataformas com o projeto base:

* Criar uma plataforma "vazia" que permita a compilação do projeto sem necessitar a importação de uma plataforma para o projeto.