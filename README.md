# **Projeto: Controle de LEDs e Display OLED com Raspberry Pi Pico**

## **📌 Descrição**
Este projeto implementa um **sistema de controle de LEDs RGB e um display OLED SSD1306** utilizando o **Raspberry Pi Pico**. Ele inclui:

✅ **Controle de LEDs Vermelho e Azul via Joystick (PWM).**  
✅ **Botão do Joystick alterna o LED Verde e modifica a borda do display.**  
✅ **Botão A liga/desliga todos os LEDs (PWM + LED Verde).**  
✅ **Display OLED exibe a posição do Joystick e muda de borda.**  
✅ **Uso de interrupções para os botões, evitando polling.**

---

## **📦 Componentes Utilizados**

| Componente | Quantidade |
|------------|------------|
| Raspberry Pi Pico | 1 |
| Joystick Analógico | 1 |
| Display OLED SSD1306 (I2C) | 1 |
| LED RGB (Vermelho, Verde e Azul) | 3 |
| Resistores (330Ω) | 3 |
| Botão para o Joystick | 1 |
| Botão A | 1 |
| Jumpers | Diversos |

---

## **📌 Esquema de Ligação**

| Componente | Pino Raspberry Pi Pico |
|------------|------------------------|
| Joystick Eixo X | GPIO 26 (ADC0) |
| Joystick Eixo Y | GPIO 27 (ADC1) |
| Botão Joystick | GPIO 22 |
| Botão A | GPIO 5 |
| LED Vermelho (PWM) | GPIO 11 |
| LED Verde (Botão Joystick) | GPIO 12 |
| LED Azul (PWM) | GPIO 13 |
| Display OLED SDA | GPIO 14 |
| Display OLED SCL | GPIO 15 |

---

## **📌 Configuração do Ambiente no VS Code**

### **1️⃣ Instalar Dependências**
No Linux/MacOS:
```sh
sudo apt update
sudo apt install cmake gcc-arm-none-eabi build-essential ninja git
```

No Windows (**via Git Bash** ou **PowerShell**):
```sh
git clone https://github.com/raspberrypi/pico-sdk.git --branch master --depth=1
cd pico-sdk
git submodule update --init
```

Depois, defina a variável de ambiente do SDK:
```sh
export PICO_SDK_PATH="caminho_para_o_pico_sdk"
```

---

### **2️⃣ Clonar este repositório e compilar o projeto**
```sh
git clone https://github.com/luizfiliperibeiro/Tarefa-1-Aula-Sincrona-10-02.git
cd Tarefa-1-Aula-Sincrona-10-02
mkdir build && cd build
cmake ..
make -j$(nproc)
```

Isso gerará um arquivo `.uf2` na pasta `build/`.

---

### **3️⃣ Subir o código para o Raspberry Pi Pico**
1. Conecte o **Raspberry Pi Pico** ao PC segurando o botão **BOOTSEL**.  
2. Ele aparecerá como um **drive USB**.  
3. **Arraste o arquivo `.uf2`** para dentro da unidade USB do Pico.  
4. O Pico reiniciará automaticamente e executará o programa. 🎉  

---

## **📌 Como Utilizar o Projeto**
- **Movimente o Joystick:** Controla o brilho dos LEDs Vermelho e Azul.  
- **Pressione o Botão do Joystick:** Alterna o LED Verde e muda a borda do display.  
- **Pressione o Botão A:** Liga/desliga todos os LEDs.  
- **Veja a posição do Joystick no display OLED.**  

---

## **📌 Manutenção e Atualização do Código**
Sempre que modificar algo, siga esses passos:

```sh
git status  # Verificar mudanças
git add .   # Adicionar arquivos modificados
git commit -m "Mensagem explicativa"
git push origin main  # Enviar para o GitHub
```

---