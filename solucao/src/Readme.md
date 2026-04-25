O repositório trata-se de um código baseado na trajetória de campos potenciais. Após a tentativa de fazer com outros métodos de trajetória, como o D*, a fluidez dos robôs foi mais visível pelo método dos campos potenciais. Este código usa 100% campos potenciais.

Como foram utilizadas as estratégias em código?

O código possui um goleiro dinâmico. Como as regras impediam os jogadores de adentrarem a área por não serem o goleiro, a bola travava em certos cantos. Assim, os atacantes que estiverem perto da bola nesse local serão reconhecidos como goleiros e, dessa forma, poderão pegar a bola na área.

Para que os robôs que forem disputar a bola na mesma distância não se prendam e acabem com a continuação do jogo, foi implementada uma quebra de simetria, onde o robô que estiver mais abaixo no eixo Y recua e assume o suporte.

Além disso, para não prender na parede, os atacantes foram projetados para que, ao caírem num extremo, desliguem o travamento de segurança e puxem a bola a 45 graus.
