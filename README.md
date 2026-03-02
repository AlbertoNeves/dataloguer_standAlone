Projeto iniciado em 2020. esta funcionando , mas todo baguncado. hoje (marco/2026) estou reescrevendo ele do zero.

estou mudando de orientado a eventos invisíveis + variáveis globais compartilhadas (tudo misturado)  para 

┌──────────────────────────┐
│        APPLICATION        │  ← telas, menus, fluxo
├──────────────────────────┤
│         SERVICES          │  ← logger, sensores, SD, RTC
├──────────────────────────┤
│          DRIVERS          │  ← botões, display, hw
└──────────────────────────┘




