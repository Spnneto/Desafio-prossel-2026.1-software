#include "Estrategia.h"
#include <cmath>

using namespace std;

Estrategia::Estrategia(int id, bool isTeamA) : id(id), teamA(isTeamA) {
    if (id == 0) role = "Goleiro";
    else if (id == 1) role = "Ala";
    else role = "Atacante";
}

Action Estrategia::think(const GameState& state) {
    Action a;
    const EntityState& eu = state.getMe();
    const EntityState& bola = state.ball;

    float lado = teamA ? -1.0f : 1.0f; 
    bool souOAtacante = false; 

    float meuGolX = 0.75f * lado;
    float posGoleiroX = eu.x;
    float posGoleiroY = eu.y;
    float menorDistGol = eu.distTo(meuGolX, 0.0f);

    for (const auto& aliado : state.teammates) {
        float dist = std::sqrt(pow(aliado.x - meuGolX, 2) + pow(aliado.y, 2));
        if (dist < menorDistGol) {
            menorDistGol = dist;
            posGoleiroX = aliado.x;
            posGoleiroY = aliado.y;
        }
    }

    if (role != "Goleiro") {
        float minhaDistBola = eu.distTo(bola.x, bola.y);
        souOAtacante = true; 

        for (const auto& aliado : state.teammates) {
            if (eu.distTo(aliado.x, aliado.y) < 0.01f) continue; 
            
            if (std::abs(aliado.x - posGoleiroX) < 0.01f && std::abs(aliado.y - posGoleiroY) < 0.01f) continue; 

            float distAliadoBola = std::sqrt(pow(aliado.x - bola.x, 2) + pow(aliado.y - bola.y, 2));
            
            if (distAliadoBola < minhaDistBola - 0.02f) {
                souOAtacante = false; 
            } else if (abs(distAliadoBola - minhaDistBola) <= 0.02f) {
                if (aliado.y < eu.y) souOAtacante = false;
            }
        }
    }
    
    if (role == "Goleiro") {
        float alvoX = 0.75f * lado;
        float alvoY = bola.y;
        if (alvoY > 0.20f) alvoY = 0.20f;
        if (alvoY < -0.20f) alvoY = -0.20f;

        float angulo = eu.angleTo(alvoX, alvoY);
        if (eu.distTo(alvoX, alvoY) > 0.05f) {
            a.moveDirectionX = std::cos(angulo);
            a.moveDirectionY = std::sin(angulo);
        } else {
            a.moveDirectionX = 0; a.moveDirectionY = 0;
        }
    }
    else if (souOAtacante) {
        float alvoX = bola.x;
        float alvoY = bola.y;

        float limiteY = 0.60f;
        float limiteX = 0.70f;

        if (bola.y > limiteY) alvoY = bola.y - 0.08f;
        else if (bola.y < -limiteY) alvoY = bola.y + 0.08f;

        if (bola.x > limiteX) alvoX = bola.x - 0.08f;
        else if (bola.x < -limiteX) alvoX = bola.x + 0.08f;

        float dx = alvoX - eu.x;
        float dy = alvoY - eu.y;
        float dist = std::sqrt(dx*dx + dy*dy);
        if(dist > 0) { dx /= dist; dy /= dist; }

        for (const auto& inimigo : state.opponents) {
            float dist_ini = eu.distTo(inimigo.x, inimigo.y);
            if (dist_ini > 0.01f && dist_ini < 0.15f) {
                float rx = (eu.x - inimigo.x) / dist_ini;
                float ry = (eu.y - inimigo.y) / dist_ini;
                dx += (-ry * 0.5f); 
                dy += (rx * 0.5f);
            }
        }

        float mag = std::sqrt(dx*dx + dy*dy);
        if(mag > 0) { dx /= mag; dy /= mag; }

        float margemMotorY = 0.62f;
        float margemMotorX = 0.72f;
        
        if (eu.y > margemMotorY && dy > 0) dy = 0.0f;
        if (eu.y < -margemMotorY && dy < 0) dy = 0.0f;
        if (eu.x > margemMotorX && dx > 0) dx = 0.0f;
        if (eu.x < -margemMotorX && dx < 0) dx = 0.0f;

        mag = std::sqrt(dx*dx + dy*dy);
        if(mag > 0) { 
            a.moveDirectionX = dx / mag; 
            a.moveDirectionY = dy / mag; 
        } else {
            a.moveDirectionX = 0; 
            a.moveDirectionY = 0;
        }
    }
    else {
        float alvoX = bola.x + (0.35f * lado);
        float alvoY = bola.y * 0.5f; 
        if ((teamA && alvoX < -0.50f) || (!teamA && alvoX > 0.50f)) alvoX = 0.50f * lado;

        float dx = alvoX - eu.x;
        float dy = alvoY - eu.y;
        float dist = std::sqrt(dx*dx + dy*dy);
        
        if (dist > 0) { dx /= dist; dy /= dist; }

        float dist_bola = eu.distTo(bola.x, bola.y);
        if (dist_bola < 0.25f) {
            dx += (eu.x - bola.x) / dist_bola * (0.05f / dist_bola);
            dy += (eu.y - bola.y) / dist_bola * (0.05f / dist_bola);
        }

        for (const auto& inimigo : state.opponents) {
            float dist_inimigo = eu.distTo(inimigo.x, inimigo.y);
            if (dist_inimigo > 0.01f && dist_inimigo < 0.15f) {
                dx += (eu.x - inimigo.x) / dist_inimigo * (0.02f / dist_inimigo);
                dy += (eu.y - inimigo.y) / dist_inimigo * (0.02f / dist_inimigo);
            }
        }

        float mag = std::sqrt(dx*dx + dy*dy);
        if (dist > 0.05f && mag > 0) {
            a.moveDirectionX = dx / mag;
            a.moveDirectionY = dy / mag;
        } else {
            a.moveDirectionX = 0; a.moveDirectionY = 0;
        }
    }

    return a;
}
