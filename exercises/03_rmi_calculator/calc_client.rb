require 'drb/drb'

class String
	def numeric?
		return true if self =~ /\A\d+\Z/
		true if Float(self) rescue false
	end
end

SERVER_URI = "druby://#{ARGV.first}"
server = DRbObject.new_with_uri(SERVER_URI)

if ARGV[1].numeric? then
	first_value = ARGV[1].to_f
	if first_value.respond_to?(ARGV[2]) then
		operator = ARGV[2]
		if ARGV[3].numeric? then
			second_value = ARGV[3].to_f
			answer = server.calculate(first_value, operator, second_value)
			puts "#{first_value} #{operator} #{second_value} = #{answer}"
		else
			raise "The second operand is not a number!"
		end
	else
		raise "Unknown operator!\nIf you want to multiply, insert the * with quotes:\nEx: 5 \'+\' 3"
	end
else
	raise "The first operand is not a number!"
end
