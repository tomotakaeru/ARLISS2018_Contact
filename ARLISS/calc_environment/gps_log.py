import serial
import os

gps_log="0"+"\t"+"0"+"\t"+"0"+"\t"+"0"
alt_log="0"

firstFixFlag = False
# change True at first fix
firstFixDate = ""


# Set up serial:
ser = serial.Serial(
    port='/dev/ttyS0',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
    timeout=1)


# Helper function to take HHMM.SS,
# Hemisphere and make it decimal:
def degrees_to_decimal(data, hemisphere):
    try:
        decimalPointPosition = data.index('.')
        degrees = float(data[:decimalPointPosition-2])
        minutes = float(data[decimalPointPosition-2:])/60
        output = degrees + minutes
        if hemisphere is 'N' or hemisphere is 'E':
            return output
        if hemisphere is 'S' or hemisphere is 'W':
            return -output
    except:
        return ""
 
# Turn $GPRMC sentence into a Python dictionary.
def parse_GPRMC(data):
    data = data.split(',')
    dict = {
            'fix_time': data[1],
            'validity': data[2],
            'latitude': data[3],
            'latitude_hemisphere': data[4],
            'longitude': data[5],
            'longitude_hemisphere': data[6],
#            'speed': data[7],
#            'true_course': data[8],
            'fix_date': data[9],
#            'variation': data[10],
#            'variation_e_w': data[11],
#            'checksum': data[12]
    }
 
    dict['decimal_latitude'] = degrees_to_decimal(dict['latitude'], dict['latitude_hemisphere'])
    dict['decimal_longitude'] = degrees_to_decimal(dict['longitude'], dict['longitude_hemisphere'])
    return dict


def parse_GPGGA(data):
    data = data.split(',')
    dict = {
            'fix_time': data[1],
            'latitude': data[2],
            'latitude_hemisphere': data[3],
            'longitude': data[4],
            'longitude_hemisphere': data[5],
            'mode': data[6],
            'satellite_number': data[7],
            'suiheiseido_teikaritu': data[8],
            'sea_level_altitude': data[9],
            'geoid_altitude': data[11],
            'checksum': data[14]
    }
 
#    dict['decimal_latitude'] = degrees_to_decimal(dict['latitude'], dict['latitude_hemisphere'])
#    dict['decimal_longitude'] = degrees_to_decimal(dict['longitude'], dict['longitude_hemisphere'])
    return dict



while True:
    line = ser.readline()
    if "$GPRMC" in line: # This will exclude other NMEA sentences the GNSS provides.
        gpsData = parse_GPRMC(line) # Turn a GPRMC sentence into a Python dictionary called gpsData
        if gpsData['validity']=="A":
            # If the sentence shows that there's a fix, then we can log the line
            if firstFixFlag is False:
                # If we haven't found a fix before, set the filename prefix with GPS date & time.
                firstFixDate = gpsData['fix_date'] + "-" + gpsData['fix_time']
                firstFixFlag = True
            
            gps_log = gpsData['fix_date'] + "\t" + gpsData['fix_time'] + "\t" + str(gpsData['decimal_latitude']) + "\t" + str(gpsData['decimal_longitude'])
            print (gps_log)

    if "$GPGGA" in line:
        altData = parse_GPGGA(line)
        if firstFixFlag is True:
            alt_log = str(altData["sea_level_altitude"])
            print (alt_log)

    if ("$GPRMC" in line or "$GPGGA" in line):
        with open("/home/pi/test/" + firstFixDate + "_gps_log.txt", "a") as f:
            f.write(gps_log + "\t" + alt_log + "\r\n")
            print ("log written")
