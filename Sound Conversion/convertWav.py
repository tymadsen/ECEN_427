import sys
import codecs
import wave
import struct

def main():
	for i in sys.argv[1:]:
		outfile(i)

def outfile(name):
	# print 'hello there'
	# name = sys.argv[2]
	
	infile = wave.open('../Wavs/AudioFiles/'+name+'.wav', 'rb')
	outfile = codecs.open(name+'.c', 'w')

	outfile.write("int "+name+"_soundData[] = {");

	(nchannels, sampwidth, framerate, nframes, comptype, compname) = infile.getparams ()
	print sampwidth
	for x in xrange(1,nframes):
		outfile.write(str(ord(infile.readframes(1))))
		if x < nframes:
			outfile.write(", ")
	
	outfile.write('};\n')

	# Write num samples
	outfile.write('int '+name+'_numberOfSamples = '+str(nframes)+';\n')
	
	# Write sample rate
	outfile.write('int '+name+'_sampleRate = '+str(framerate)+';\n')

	# Close files
	infile.close()
	outfile.close()

if __name__ == '__main__':
	main()