# Controle de Fase para Carga AC com Arduino

Este projeto simula o controle de potência em cargas AC (como a resistência de um chuveiro), utilizando Arduino, detecção de zero crossing e atraso no disparo de um triac. A **branch `pratica`** contém a versão adaptada usada em um experimento real, com alimentação de 9V por transformador para maior segurança.

## 🛠️ Setup – SimulIDE

### 1. Copie a pasta `user_data`

Essa pasta contém subcircuitos personalizados e componentes adicionais para o SimulIDE, incluindo um **opto-triac simulado** necessário para o controle de potência em AC.

- Copie a pasta `user_data` deste repositório.

### 2. Cole na pasta do SimulIDE

- Vá até o diretório onde o SimulIDE está instalado ou onde deseja manter os dados personalizados.
- Cole a pasta `user_data` nesse local.

> Exemplo: Se o SimulIDE está em `C:\Program Files\SimulIDE`, cole a pasta `user_data` lá (ou em outro diretório de sua escolha).

### 3. Configure o SimulIDE para usar a pasta `user_data`

1. Abra o **SimulIDE**.
2. Vá em **Settings** (ou **Configurações**) > **App**.
3. Encontre a opção **User Data Folder**.
4. Clique para selecionar uma nova pasta.
5. Escolha a pasta `user_data` que você colou anteriormente.

**Importante:** Após configurar a pasta, reinicie o SimulIDE para que os novos componentes sejam carregados corretamente.

## ⚡ Circuito

![phase-control-circuit](https://github.com/user-attachments/assets/6bc23a70-8b64-4a1a-ba4a-c70a5d7a50bb)

## 🔍 Funcionamento

### 1. Detecção do zero crossing

Um retificador de onda completa transforma a forma de onda AC em uma forma sempre positiva, com vales próximos a 0V nos momentos de cruzamento por zero da senoide. Esse sinal é conectado ao **pino 2 do Arduino**, que detecta quedas de tensão para sincronizar o disparo do triac com a rede elétrica.

**Resumo:** essa etapa identifica os momentos exatos em que a tensão da rede elétrica cruza o zero.

### 2. Geração do pulso de controle

Quando o Arduino detecta um cruzamento por zero, uma **interrupção externa** é disparada:

- A interrupção coloca o **pino 9** em nível baixo (LOW) e inicia o **Timer1**.
- O registrador **OCR1A** define o tempo de atraso até o início do pulso, com base no valor do potenciômetro no **pino A0**.
- Ao atingir OCR1A, o **pino 9** vai para HIGH, iniciando o pulso.
- O valor de **OCR1B** é então ajustado para OCR1A + duração do pulso.
- Quando o timer atinge OCR1B, o **pino 9 volta para LOW**, finalizando o pulso.

**Resumo:** essa etapa gera um pulso com atraso controlável em relação ao cruzamento por zero, que determina o ponto de disparo do triac.

### 3. Ativação da carga AC com optotriac e triac

O pulso no **pino 9** ativa um **opto-triac**, que aciona um **triac de potência** ligado à carga. O opto-triac garante o isolamento entre o Arduino e a rede elétrica.

O triac permanece conduzindo até que a corrente da rede naturalmente chegue a zero, desligando-se automaticamente.

**Resumo:** essa etapa permite o acionamento seguro e sincronizado da carga AC, controlando a potência entregue de acordo com o tempo de atraso configurado.

## Simulação

https://github.com/user-attachments/assets/db5c947f-280a-47d7-b454-f483f6917f35

**Osciloscópio esquerdo**  
- Linha amarela — onda da rede elétrica  
- Linha cinza — sinal de detecção do cruzamento por zero no pino 2 do Arduino  
- Linha laranja — pulso com atraso controlável gerado  

**Osciloscópio direito**  
- Linha amarela — onda da rede elétrica cortada  

## Referências

- Código baseado no repositório: [AC Phase Control – GitHub](https://github.com/chayanforyou/AC-Phase-Control)
  
- Circuito baseado no tutorial: [How2Electronics – AC Dimmer](https://how2electronics.com/220v-ac-light-fan-dimmer-using-triac-arduino/)
