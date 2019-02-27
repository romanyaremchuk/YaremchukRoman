# class Book():
#
#     def __init__(self, title, author, pages):
#         self.title = title
#         self.author = author
#         self.pages = pages
#
#     def __str__(self):
#         return "title: {}\n author: {}\n pages: {}".format(self.title, self.author,self.pages)
#
#     def __len__(self):
#         return self.pages
#
#     def __del__(self):
#         print("a book is destroyed!")
# b = Book("python","jose", 200)
# del b


SUITE = 'H D S C'.split()
RANKS = '2 3 4 5 6 7 8 9 10 J Q K A'.split()


class Deck():
    def __init__(self, S, R):
        self.S = S
        self.R = R

    def DCreate(self):
        cardList = ''
        for ss in range(0, len(S)):
            for rr in range(0, len(R)):
                cardList = cardList + S[ss] + R[rr] + " "
        print(cardList.split())
