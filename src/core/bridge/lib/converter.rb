#!/usr/bin/ruby

module KeyRemap4MacBookBridge
  class Converter
    def self.update_file_if_needed(tmpfilepath)
      targetfilepath = tmpfilepath.gsub(/\.tmp$/, '')

      if (! FileTest.exist?(targetfilepath)) or (IO.read(tmpfilepath) != IO.read(targetfilepath)) then
        File.rename(tmpfilepath, targetfilepath)
      else
        File.unlink(tmpfilepath)
      end
    end
  end
end
