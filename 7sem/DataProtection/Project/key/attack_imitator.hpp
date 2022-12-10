#pragma once

#define RADIUS_INCREASING_ATTACK 4
#define KEY_IMITATION_ATTACK 5
#define WRONG_CRYPTO_KEY 6

bool RadiusIncreasingAttack() { return digitalRead(RADIUS_INCREASING_ATTACK); }
bool KeyImitationAttack() { return digitalRead(KEY_IMITATION_ATTACK); }
bool WrongCryptoKeyAttack() { return digitalRead(WRONG_CRYPTO_KEY); }

