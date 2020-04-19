#!/usr/bin/env ruby
# frozen_string_literal: true

require 'rubygems'
require 'bundler/setup'
require 'csv'
require 'nokogiri'

fname = ARGV[0]

unless fname
  puts 'Usage: mklevel.rb FILE'
  return
end

unless fname.end_with? '.tmx'
  puts "#{fname} is not a TMX file."
  return
end

puts "reading #{fname}..."
doc = File.open(fname) { |f| Nokogiri::XML(f) }

basename = `basename #{fname}`.strip

map = doc.at_css('map')
height = map['height']
width = map['width']

layers = []

map.xpath('//layer').each do |layer|
  tag = layer.attr('name')
  data = layer.at_css('data').text
  csv_data = CSV.parse(data)
  csv_data.shift # skip header

  l = { tag: tag, content: [] }
  csv_data.each do |row|
    c = []
    row.each do |tile|
      next unless tile

      t = [0, tile.to_i - 1].max
      # 4 character hex string == 16 bits
      t_h = format('0x%<t>04X', t: t)
      c.push(t_h)
    end
    l[:content].push(c)
  end
  layers.push(l)
end

lname = basename.downcase.gsub(/\.tmx$/, '')
lname_path = fname.gsub(/\.[Tt][Mm][Xx]$/, '')
l_u_name = lname.upcase
l_h_name = lname + '.h'
l_c_name = lname + '.c'

puts "#{lname} is #{width}x#{height}..."

puts "writing #{l_u_name} to #{l_h_name} and #{l_c_name}..."

l_const_str = ''
l_data_str = ''

total_size = width.to_i * height.to_i
total_len = total_size * 4 # TODO

layers.each do |layer|
  l_const_str += "extern const uint16_t #{l_u_name}_#{layer[:tag].upcase}[#{total_size}];\n"

  l_data_str += "const uint16_t #{l_u_name}_#{layer[:tag].upcase}[#{total_size}] = {\n"
  layer[:content].each do |row|
    l_data_str += row.join(', ') + ",\n"
  end
  l_data_str.gsub!(/, \n$/, '')
  l_data_str += "\n};\n\n"
end

File.open(lname_path + '.h', 'w') do |file|
  file.write(%(
#pragma once

#include <stdint.h>

#define #{l_u_name}_LEN #{total_len}
#define #{l_u_name}_HEIGHT #{height}
#define #{l_u_name}_WIDTH #{width}

#{l_const_str}
  ).strip)
end

File.open(lname_path + '.c', 'w') do |file|
  file.write(%(
#include "#{l_h_name}"

#include <stdint.h>

#{l_data_str}
  ).strip)
end
