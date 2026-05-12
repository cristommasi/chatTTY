class User:

	def __init__(self, name, p, q, e):
		
		self.name = name

		self.p = p
		self.q = q

		self.n = p * q

		self.phi = (self.p - 1) * (self.q - 1)

		self.e = e

		self.d = self.getPrivExponent()

		self.publicKey = [self.e, self.n]
		self.privateKey = [self.d, self.n]

	def getPrivExponent(self):
		d = 1
		while (self.e * d) % self.phi != 1:
			d += 1
		return d

	def getPublicKey(self):
		return self.publicKey

	def getPrivateKey(self):
		return self.privateKey
	
	def encryptMessage(self, message, target):
		cipher = []

		for i in range(len(message)):
			char = ord(message[i])

			encrypted = pow(char, target.publicKey[0], target.publicKey[1])

			cipher.append(encrypted)

		return (cipher)

	def decryptMessage(self, cipher):
		message = ""

		for i in range(len(cipher)):
			decrypted = pow(cipher[i], self.privateKey[0], self.privateKey[1])

			message += chr(decrypted)

		print(f"Message = '{message}'\n")

		return (message)
	


bob = User("Bob", 17, 11, 7)

alice = User("Alice", 13, 19, 5)

greta = User("Greta", 53, 23, 5)

cipher = alice.encryptMessage(
	"Hello how are you Bob",
	bob
)

bob.decryptMessage(cipher)

greta.decryptMessage(cipher)