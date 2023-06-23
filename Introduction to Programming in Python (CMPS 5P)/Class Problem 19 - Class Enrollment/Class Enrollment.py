class Course:
    students = []
    def __init__(self, name):
        self.name = name

    def enroll(self, student):
        self.students.append(student)

cs5p = Course("Introduction to Python")
print(cs5p.name)
cs5p.enroll("Jane Smith")
print(cs5p.students)
