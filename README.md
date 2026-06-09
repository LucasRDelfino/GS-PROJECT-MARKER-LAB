# 🦾 Braço Robótico de Coleta de Amostras — Docking & Retrieval

> Projeto desenvolvido para a disciplina Maker Lab  
> Manipulação de carga em ambiente de microgravidade com controle via Monitor Serial

---

## 👥 Integrantes

| Nome | RM |
|------|----|
| Gustavo Vegi | RM550188 |
| Pedro Henrique Silva de Morais | RM98804 |
| Lucas Rodrigues Delfino | RM550196 |
| Luisa Cristina dos Santos Neves | RM551889 |
| Gabriel Aparecido Cassalho Xavier | RM99794 |

---

## 🔗 Acesso ao Simulador

Projeto público no Tinkercad:  
👉 [https://www.tinkercad.com/things/bYTHgh0eR0z-gs-esr4?sharecode=XYdlZvga6Ik6aP1OE8Q8of3v10UPfeLm8zncRtYK__g](https://www.tinkercad.com/things/bYTHgh0eR0z-gs-esr4?sharecode=XYdlZvga6Ik6aP1OE8Q8of3v10UPfeLm8zncRtYK__g)

---

## 🎮 Guia de Operação

O controle é feito pelo **Monitor Serial** do Arduino IDE (baud rate: `9600`).  
Digite o comando e pressione **Enter**. Letras maiúsculas e minúsculas são aceitas.

| Comando | Ação |
|---------|------|
| `U` | **Up** — Braço sobe (servo de elevação vai para 90°) |
| `D` | **Down** — Braço desce (servo de elevação vai para 0°) |
| `O` | **Open** — Garra abre (servo da garra vai para 180°) |
| `C` | **Close** — Garra fecha e captura a amostra (servo da garra vai para 90°) |

> **LED de status:** pisca em modo repouso (idle) e acende fixo durante a execução de um comando.

---

## 🖨️ Software de Modelagem 3D

As peças do braço robótico foram modeladas no **Autodesk Fusion 360**.  
O projeto contém os seguintes corpos: `Base`, `Suporte`, `Garra1` e `Garra2`.

Arquivos entregues:
- `.f3d` — arquivo nativo do Fusion 360 (editável)
- `.stl` — arquivo exportado para impressão 3D (binário)

---

## ⚙️ Especificações Técnicas

### Pinagem — Arduino UNO

| Pino | Componente | Função |
|------|-----------|--------|
| `7` | Servo 1 (Braço) | Controle de elevação do braço |
| `8` | Servo 2 (Garra) | Controle de abertura/fechamento da garra |
| `13` | LED de status | Indicação visual de estado (idle / executando) |

### Alimentação

| Parâmetro | Valor |
|-----------|-------|
| Fonte externa | 4 pilhas AA em série |
| Tensão de saída | ~6V |
| Componentes alimentados | Servomotores (via protoboard) |
| Alimentação do Arduino | USB (via computador) |

> Os servomotores são alimentados diretamente pela fonte externa de 6V para garantir torque suficiente, enquanto o Arduino recebe alimentação pelo cabo USB. O GND da fonte e do Arduino são compartilhados na protoboard.
