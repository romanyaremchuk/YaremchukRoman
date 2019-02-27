#####################################
### WELCOME TO YOUR OOP PROJECT #####
#####################################


from random import shuffle
import os
# Two useful variables for creating Cards.
SUITE = 'Чр Бб Кр Пк'.split()
RANKS = '2 3 4 5 6 7 8 9 10 J Q K A'.split()
        #0 1 2 3 4 5 6 7 8  9 10 11 12

class Deck():

    def __init__(self, S, R, arr):
        self.S = S
        self.R = R
        self.arr = arr

    def DCreate(self):
        cardList = ''
        for s in range(0,len(self.S)):
            for r in range(0, len(self.R)):
                cardList = cardList + self.S[s] +  self.R[r] + ' '
        cardList = cardList.split()
        shuffle(cardList)
        return cardList

    def ForPC(self, arr):
        pccards = ''
        for item in range(0, 26):
            pccards = pccards  + arr[item] + " "
        # print(pccards)
        return pccards

    def ForPlayer(self, arr):
        Playercards = ''
        for item in range(26, 52):
            Playercards = Playercards + arr[item] + " "
        # print(Playercards)
        return Playercards

class Hand():
    '''
    This is the Hand class. Each player has a Hand, and can add or remove
    cards from that hand. There should be an add and remove card method here.
    '''


    def __init__(self, PCC, PlayerC):
        self.PCC = PCC
        self.PlayerC = PlayerC

    #задуматься о количестве карт минимальном
    def FromPCToPlayer(self, PCC, PlayerC): # КОМПЬЮТЕР   -----> ИГРОК
        temp0 = PCC[len(PCC) - 1]
        PlayerC.insert(0,temp0)

        temp0 = PlayerC[len(PlayerC) - 1] #игрока карта которая была на столе -- в конце его колоды
        PlayerC.insert(0,temp0)

        return PlayerC

    def FromPlayerToPC(self, PCC, PlayerC):
        temp = PlayerC[len(PlayerC) - 1]
        PCC.insert(0, temp) #отсюда был убран селф

        temp = PCC[len(PCC) - 1]
        PCC.insert(0, temp)

        return PCC

    def DeleteLast(self, PCC):
        helpArray = ''
        for item in range(0, len(PCC) - 1):
            helpArray = helpArray + ' ' + PCC[item] + " "
        #OK тут не пропадает
        return helpArray.split()
##
#нужен рефреш колоды у того, кто её забрал , тк одинаковая карта
#
#
##


class Player():
    """
    This is the Player class, which takes in a name and an instance of a Hand
    class object. The Payer can then play cards and check if they still have cards.
    """
    def __init__(self, PCC, PlayerC):
        self.PCC = PCC
        self.PlayerC = PlayerC

    def CheckCards(self, PCC, PlayerC):
        if len(PCC) != 0:
            print("PC still has cards!")
        #add GAMEOVER
        print("You have:")
        print(len(PlayerC))
        return len(PlayerC) - 1

    def PutCard(self, PCC, PlayerC):
        print("Are you ready to put a card?")


        input()
        clear = lambda: os.system('cls')
        clear()

        print("\n\n\n\t Your Card: %s" % (PlayerC[len(PlayerC) - 1])) #мб массив aout of range
        print("\t PC's Card: %s\n\n\n" % (PCC[len(PCC) - 1]))
        print("PC: %d" % len(PCC))
        print("Player: %d" % len(PlayerC))

######################
#### GAME PLAY #######
######################
print("Welcome to War, let's begin...")

cards = Deck(SUITE, RANKS, '')

AllCards = cards.DCreate()

PCC = cards.ForPC(AllCards).split()
PlayerC = cards.ForPlayer(AllCards).split()

h = Hand(PCC, PlayerC)

# PCC = h.FromPlayerToPC(PCC, PlayerC) #карту дать компу
# PlayerC = h.DeleteLast(PlayerC)

p = Player(PCC, PlayerC)


while(len(PCC)!= 0 or len(PlayerC != 0)):
    # print(PCC)
    # print(PlayerC)
    p.PutCard(PCC, PlayerC)
    if PCC[len(PCC) - 1][2] == '1' and PlayerC[len(PlayerC) - 1][2] == '1':
        print("oops its the same")
        if PCC[len(PCC) - 2][2] != PlayerC[len(PlayerC) - 2][2]:
            if RANKS.index(PlayerC[len(PlayerC) - 2][2]) > 8:
                print("!10! у игрока больше")
                PlayerC = h.FromPCToPlayer(PCC, PlayerC)
                PCC = h.DeleteLast(PCC)
                PlayerC = h.DeleteLast(PlayerC)
                continue
            if RANKS.index(PlayerC[len(PlayerC) - 2][2]) < 8:
                print("!10! у игрока меньше")
                PCC = h.FromPlayerToPC(PCC, PlayerC)
                PlayerC = h.DeleteLast(PlayerC)
                PCC = h.DeleteLast(PCC)
                continue
        # count = 2
        # while(True):
        #     if PCC[len(PCC) - count][2] == PlayerC[len(PlayerC) - count][2]:
        #         count += 1
        #         continue
        #     if PCC[len(PCC) - count][2] == '1':
        #         if RANKS.index(PlayerC[len(PlayerC) - count][2]) > 8:
        #             print("ОДИНАКОВО!10! у игрока больше")
        #             for item in range(0,count):
        #                 PlayerC = h.FromPCToPlayer(PCC, PlayerC)
        #                 PCC = h.DeleteLast(PCC)
        #                 PlayerC= h.DeleteLast(PlayerC)
        #             break


        ####

    if PCC[len(PCC) - 1][2] == '1':
        if RANKS.index(PlayerC[len(PlayerC) - 1][2]) > 8:
            print("!10! у игрока больше")
            PlayerC = h.FromPCToPlayer(PCC, PlayerC)
            PCC = h.DeleteLast(PCC)
            PlayerC = h.DeleteLast(PlayerC)
            continue
            ####
        if RANKS.index(PlayerC[len(PlayerC) - 1][2]) < 8:
            print("!10! у игрока меньше")
            PCC = h.FromPlayerToPC(PCC, PlayerC)
            PlayerC = h.DeleteLast(PlayerC)
            PCC = h.DeleteLast(PCC)
            continue
            ####

    if PlayerC[len(PlayerC) - 1][2] == '1':
        if RANKS.index(PCC[len(PCC) - 1][2]) > 8:
            print("!10! у компа больше")
            PCC = h.FromPlayerToPC(PCC, PlayerC)
            PlayerC = h.DeleteLast(PlayerC)
            continue
            ####
        if RANKS.index(PCC[len(PCC) - 1][2]) < 8:
            print("!10! у компа меньше")
            PlayerC = h.FromPCToPlayer(PCC, PlayerC)
            PCC = h.DeleteLast(PCC)
            continue
            ####

    if RANKS.index(PlayerC[len(PlayerC) - 1][2]) > RANKS.index(PCC[len(PCC) - 1][2]):
        print("у игрока больше")
        PlayerC = h.FromPCToPlayer(PCC, PlayerC)
        PCC = h.DeleteLast(PCC)
        PlayerC = h.DeleteLast(PlayerC)
        continue
        ####

    if RANKS.index(PlayerC[len(PlayerC) - 1][2]) < RANKS.index(PCC[len(PCC) - 1][2]):
        print("у компа больше")
        PCC = h.FromPlayerToPC(PCC, PlayerC)
        PlayerC = h.DeleteLast(PlayerC)
        PCC = h.DeleteLast(PCC)
        continue
        ####

    if RANKS.index(PlayerC[len(PlayerC) - 1][2]) == RANKS.index(PCC[len(PCC) - 1][2]):
        print("одинаково")
        PCC = h.FromPlayerToPC(PCC, PlayerC)
        PlayerC = h.DeleteLast(PlayerC)
        PCC = h.DeleteLast(PCC)
        continue
        ####
# print(RANKS.index(PCC[len(PCC) - 1][2]))
# Use the 3 classes along with some logic to play a game of war!
########
# count = 2
# while(True):
#     if RANKS.index(PlayerC[len(PlayerC) - count][2]) == RANKS.index(PCC[len(PCC) - count][2]):
#         count += 1
#         continue
#     if PCC[len(PCC) - count][2] == '1':
#         if RANKS.index(PlayerC[len(PlayerC) - count][2]) > 8:
#             for item in range(0,count):
#                 PlayerC = h.FromPCToPlayer(PCC, PlayerC)
#                 PCC = h.DeleteLast(PCC)
#     count = 2
#     break
#     print("TEST")
