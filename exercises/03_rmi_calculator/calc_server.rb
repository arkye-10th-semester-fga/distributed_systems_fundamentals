require 'drb/drb'

class CalculatorServer
	def calculate(first_value, operator, second_value)
		puts "Request: #{first_value} #{operator} #{second_value}"
		answer = first_value.public_send(operator, second_value)
		puts "- Answer: #{answer}"
		return answer
	end
end

URI = "druby://#{ARGV.first}"
FRONT_OBJECT = CalculatorServer.new

puts 'Starting server...'
DRb.start_service(URI, FRONT_OBJECT)
puts "Listening on #{URI}"
DRb.thread.join
