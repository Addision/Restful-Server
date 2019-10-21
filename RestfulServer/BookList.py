
class BookList:
    def __init__(self):
        self.booklist = []
        
    def Insert(self, book_id):
        if not book_id:
            return None
        if book_id not in self.booklist:
            self.booklist.append(book_id)
        else:
            self.booklist.remove(book_id)
            self.booklist.insert(0, book_id)
    
    def Remove(self, book_id):
        if not book_id:
            return None
        if book_id not in self.booklist:
            return
        self.booklist.remove(book_id)
        
    def GetIndex(self, book_id):
        if not book_id:
            return None
        if book_id not in self.booklist:
            return None
        return self.booklist.index(book_id)
    
    def __str__(self):
        return "|".join(self.booklist)
        