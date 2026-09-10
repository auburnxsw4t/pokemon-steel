#!/usr/bin/env python3
"""Exercise all Steel starter branches and both early-rival outcomes in mGBA."""
import sys

from test_steel_opening_emulator import Emulator


CHOICES = (
    # name, ball x, player choice, Kyle choice, player species, Kyle species,
    # forced battle outcome, hidden ball flags
    ('POSSKIT', 7, 1, 3, 'species_zigzagoon', 'species_taillow', 1, (0x29, 0x2b)),
    ('SHELDO', 8, 2, 1, 'species_sandshrew', 'species_zigzagoon', 2, (0x2a, 0x29)),
    ('MIMBRI', 9, 3, 2, 'species_taillow', 'species_sandshrew', 1, (0x2b, 0x2a)),
)


def wait_for_battle(emulator, player_species, kyle_species):
    seen = set()
    for _ in range(1600):
        flags = emulator.u32(emulator.sym('gBattleTypeFlags'))
        if flags:
            pair = (emulator.battle_species(0), emulator.battle_species(1))
            if pair not in seen:
                print('battle', hex(flags), pair,
                      'parties', emulator.trainer_party_species(0), emulator.trainer_party_species(1),
                      flush=True)
                seen.add(pair)
            if pair == (player_species, kyle_species):
                return
        emulator.step(2, 1)
        emulator.step(18)
    raise AssertionError(('rival battle did not start', emulator.stage(), emulator.location()))


def finish_opening(emulator):
    emulator.advance(lambda: emulator.stage() == 15, limit=40000)
    # Let the departure movement, actor sync, and releaseall finish.
    emulator.step(240)


def run_choice(runner, choice):
    name, x, player_choice, kyle_choice, player_attr, kyle_attr, outcome, hidden_flags = choice
    emulator = Emulator(runner)
    emulator.step(400)
    emulator.tap(8)
    try:
        emulator.advance(lambda: emulator.stage() == 12)
        emulator.step(120)
        # Approach the display around Kyle's starting tile at (7, 6).
        emulator.goto(6, 4)
        emulator.goto(x, 4)
        emulator.step(20)
        emulator.walk(64, 16)
        emulator.tap(1)
        emulator.advance(lambda: emulator.stage() == 13)

        player_species = getattr(emulator, player_attr)
        kyle_species = getattr(emulator, kyle_attr)
        assert emulator.var(0xf8) == player_choice, (name, 'player choice', emulator.var(0xf8))
        assert emulator.var(0xf9) == kyle_choice, (name, 'Kyle choice', emulator.var(0xf9))
        assert emulator.party_count() == 1, (name, 'party count', emulator.party_count())
        assert emulator.party_species() == player_species, (name, 'party species', emulator.party_species())

        wait_for_battle(emulator, player_species, kyle_species)
        if outcome == 1:
            emulator.set_battle_hp(1, 1)
        else:
            emulator.set_battle_hp(0, 1)
        finish_opening(emulator)

        assert emulator.u32(emulator.sym('gBattleOutcome')) & 0xff == outcome, (name, 'outcome')
        assert emulator.party_count() == 1, (name, 'party lost')
        assert emulator.party_species() == player_species, (name, 'starter changed')
        assert emulator.party_hp() == emulator.party_max_hp() > 0, (name, 'starter not healed')
        assert emulator.flag(0x2c), (name, 'Silk Scarf flag')
        assert emulator.item_quantity(emulator.item_silk_scarf) >= 1, (name, 'Silk Scarf missing')
        assert all(emulator.flag(flag) for flag in hidden_flags), (name, 'chosen balls visible')
        remaining = {0x29, 0x2a, 0x2b} - set(hidden_flags)
        assert len(remaining) == 1 and not emulator.flag(remaining.pop()), (name, 'Logan ball hidden')
        emulator.assert_kyle('REGISTRATION')

        # Normal control is restored, and home can be exited and re-entered.
        emulator.goto(8, 7)
        emulator.walk(128, 32)
        emulator.step(120)
        assert emulator.location()[1] == 1, (name, 'home exit', emulator.location())
        emulator.walk(64, 32)
        emulator.step(120)
        assert emulator.location()[1] == 2 and emulator.stage() == 15, (name, 'home re-entry')
        emulator.assert_kyle('REGISTRATION')

        # Kyle's sole active instance is placed at League Registration.
        emulator.goto(8, 7)
        emulator.walk(128, 32)
        emulator.step(120)
        emulator.goto(8, 16)
        emulator.goto(35, 16)
        emulator.walk(16, 40)
        emulator.step(120)
        assert emulator.location()[1] == 4, (name, 'Alumina entry')
        emulator.goto(26, 27)
        emulator.goto(32, 27)
        emulator.step(60)
        assert emulator.object_position(2) == (33, 27), (name, 'Kyle registration position', emulator.object_position(2))
        print(f'PASS: {name}, Kyle counter, outcome {outcome}, reward, healing, and home re-entry')
    finally:
        emulator.close()


def main():
    for choice in CHOICES:
        run_choice(sys.argv[1], choice)


if __name__ == '__main__':
    main()
