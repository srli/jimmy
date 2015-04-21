output = 'banana'

def store_output(outputValue):
	output = outputValue
	return output

def get_output():
	return output

def store_counter(counterValue):
	global counter
	counter = counterValue
	return counter

def get_counter():
	return counter

store_output('hello')
print get_output()