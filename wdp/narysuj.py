# Skrypt zakłada, że podany napis jest poprawny (w postaci określonej w zadaniu).
# Może zachowywać się w nieprzewidziany sposób, jeśli wejście będzie niepoprawne.

import argparse 
import turtle 

obrotStopnie = {"A": 0, "B": 60, "C": 120, "D": 180, "E": 240, "F": 300}
parser = argparse.ArgumentParser(
    description="Program Bajtka do rysowania trójkątów. "
    "Aby użyć wywołać komendę: python3 narysuj.py "
    "   lub    python3 narysuj.py <plik.out")
parser.add_argument("--speed", type=int, default=3, 
    help="Prędkość tworzenia się rysunku, domyślnie 3 jednostki. "
    "Aby to zmienić, użyj flagi --speed NOWA_PREDKOSC")
parser.add_argument("--pensize", type=int, default=5, 
    help="Grubość kreski, domyślnie 5 jednostek. "
    "Aby to zmienić, użyj flagi --pensize NOWA_GRUBOSC")
parser.add_argument("--side", type=int, default=40, 
    help="Dlugość boku trójkąta, domyślnie 40 jednostek. "
    "Aby to zmienić, użyj flagi --side NOWA_DLUGOSC")
parser.add_argument("--pencolor", type=str, default="black", 
    help="Kolor kreski, domyslnie czarny (black). "
    "Aby to zmienić, użyj flagi --pencolour NOWY_KOLOR. "
    "Kolor musi być zapisany w języku angielskim.")
args = parser.parse_args()

def wykonajRuch(c):
    turtle.rt(obrotStopnie[c])
    turtle.fd(args.side)
    turtle.lt(obrotStopnie[c])


def policzNawiasy(S):
    stos = []
    nawiasy = {}
    for i in range(len(S)):
        c = S[i]
        if c == "[":
            stos.append(i)
        if c == "]":
            nawiasy[stos[-1]] = i
            stos.pop()
    return nawiasy


def czyPoprawnaLitera(c):
    return "A" <= c <= "F"


def czyPoprawnaCyfra(c):
    return "1" <= c <= "9"


def symuluj(S, nawiasy, num, beg, end):
    for iter in range(num):
        i = beg
        while i < end:
            c = S[i]
            if c == "[":
                symuluj(S, nawiasy, int(S[i - 1]), i + 1, nawiasy[i])
                i = nawiasy[i]
            if czyPoprawnaLitera(c):
                repeatCnt = 1
                if i > 0 and czyPoprawnaCyfra(S[i - 1]):
                    repeatCnt = int(S[i - 1])
                for _ in range(repeatCnt):
                    wykonajRuch(c)
            i += 1


def rysuj(S):
    turtle.speed(args.speed)
    turtle.pd()
    turtle.pencolor(args.pencolor)
    turtle.pensize(args.pensize)
    nawiasy = policzNawiasy(S)
    symuluj(S, nawiasy, 1, 0, len(S))



rysuj(input())
