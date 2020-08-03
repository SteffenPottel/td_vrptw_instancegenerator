# scrip for generation of charging points

#############################################################

import random 
rng = random.Random()  

import pandas as pd

import sys
import os

#############################################################

def eucl_dist(x1,y1,x2,y2):
	return ( (x1-x2)**2 + (y1-y2)**2 )**0.5

#############################################################

def usage():
	"""
	Explain correct script call
	"""
	print("Please use the following command: <Python-Interpreter> instance_generator.py <Solomon instance folder> <number of service stations per quadrant> <charging time> <battery capacity> <Output folder>")
	
	return None

def read_file(filename):
	"""
	Collect all data from a given file
	with read_csv-method of pandas
	Input:
	path to Solomon file
	Output:
	list of depot and customer data
	"""
	result = []

	data = pd.read_csv(filename, header = 5)
	datalist = data.values.tolist()
	for element in datalist:
		result.append(element[0].split())

	return result

def generate_coordinates_r(count):
	"""
	generate and return a given number (count)
	of coordinates for service stations
	randomly distributed in a 50 units radius
	"""
	result = []

	while(len(result) < count):
		x = rng.randint(0,100)
		y = rng.randint(0,100)
		if (eucl_dist(x,y,50,50) < 50):
			check = True
			for elem in result:
				if (eucl_dist(x,y, elem[0], elem[1]) < 10):
					check = False
			if check:
				result.append( (x,y) )

	return result

def generate_coordinates_c(count):
	"""
	generate and return a given number (count)
	of coordinates for service stations
	distributed around congestion centers
	NOTE: count should be divisible by the 
	length of centers
	"""
	result = []
	centers = [(25,25), (25,75), (75,25), (75,75)]
	partial_count = count//len(centers)

	for center in centers:
		subresult = []
		while(len(subresult) < partial_count):
			x = rng.randint(0,100)
			y = rng.randint(0,100)
			if (eucl_dist(x,y,center[0],center[1]) < 15):
				subresult.append( (x,y) )
		for elem in subresult:
			result.append(elem)

	return result

def generate_instance(name, solomon_data, service_stations, service_time, battery, capacity):
	"""
	compile the whole instance and
	write it to name-file
	NOTE: solomon data entry 0 is the depot!
	"""
	output = "ID\tType\tx\ty\tDemand\tReady\tDue\tService\n"
	output += "D" + solomon_data[0][0] + "\td\t" 
	for index in range(1,7):
		output += solomon_data[0][index] + "\t"
	output += "\n"
	output += "S" + solomon_data[0][0] + "\tf\t" 
	for index in range(1,6):
		output += solomon_data[0][index] + "\t"
	output += str(service_time) + "\n"
	counter = 1
	for element in service_stations:
		output += "S" + str(counter) + "\tf\t" + str(element[0]) + "\t" + str(element[1]) + "\t"
		for index in range(3,6):
			output += solomon_data[0][index] + "\t"
		output += str(service_time) + "\n"
		counter += 1
	for index_out in range(1,101):
		output += "C" + solomon_data[index_out][0] + "\tc\t" 
		for index_in in range(1,7):
			output += solomon_data[index_out][index_in] + "\t"
		output += "\n"
	output += "\n"
	output += "Q battery capacity /" + str(battery) + "/ \n"
	output += "C vehicle load /" + str(capacity) + "/ \n"
	output += "R replenishment time /" + str(service_time) + "/ \n"

	with open (name, 'w') as file_handle:
		file_handle.write(output)

##############################################################

c1_num = 9
c2_num = 8
r1_num = 12
r2_num = 11
rc1_num = 8
rc2_num = 8
instance_numbers = [c1_num, c2_num, r1_num, r2_num, rc1_num, rc2_num]

c1_pre = 'c1'
c2_pre = 'c2'
r1_pre = 'r1'
r2_pre = 'r2' 
rc1_pre = 'rc1'
rc2_pre = 'rc2'
prefixes = [c1_pre, c2_pre, r1_pre, r2_pre, rc1_pre, rc2_pre]

capacities = [200, 700, 200, 1000, 200, 1000]

suffix = '.txt'

##############################################################

if __name__ == '__main__':
	
	if (len(sys.argv) != 6):
		usage()
		exit()

	in_directory = sys.argv[1]
	num_of_servicestations = 4 * int(sys.argv[2])
	service_time = int(sys.argv[3])
	battery = int(sys.argv[4])
	out_directory = sys.argv[5]

	if not os.path.exists(out_directory):
    		os.makedirs(out_directory)

	service_stations = generate_coordinates_c(num_of_servicestations)

	for index in range(6):
		capacity = capacities[index]
		for count in range(1,instance_numbers[index]+1):
			#get input directory
			input_file = in_directory + prefixes[index]
			if count < 10:
				input_file += "0" + str(count)
			else:
				input_file += str(count)
			input_file += suffix

			#write instance file
			solomon_instance = read_file(input_file)
			output_file = out_directory + prefixes[index]
			if count < 10:
				output_file += "0" + str(count)
			else:
				output_file += str(count)
			output_file += "_" + str(num_of_servicestations) + suffix
			generate_instance(output_file, solomon_instance, service_stations, service_time, battery, capacity)

