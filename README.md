# mruby-os [![Build Status](https://travis-ci.org/appPlant/mruby-os.svg?branch=master)](https://travis-ci.org/appPlant/mruby-os) [![Build status](https://ci.appveyor.com/api/projects/status/9o4do8f94gh0kagg?svg=true)](https://ci.appveyor.com/project/katzer/mruby-os)

Determine system and binary capabilities at runtime.

## Installation

Add the line below to your `build_config.rb`:

```ruby
conf.gem 'mruby-os'
```

## Usage

To get the machine architecture:
```ruby
OS.machine # i686 or x86_64
```

To get the max "word size" of the machine:
```ruby
OS.bits # 32 or 64
# or
OS.bits :machine
```

To get the used "word size" by the binary:
```ruby
OS.bits :binary
```

To get the kernel name:
```ruby
OS.sysname # Darwin, Linux or Windows_NT
```

To execute code per platform:
```ruby
if OS.posix?
  # The normal way
else
  # Windows
end
```

See also `linux?`, `mac?`, `macos?`, `osx?`, `x?`.

## Authors

- Sebastián Katzer, Fa. appPlant GmbH


## License

The gem is available as open source under the terms of the [MIT License][license].

Made with :yum: from Leipzig

© 2017 [appPlant GmbH][appplant]

[license]: http://opensource.org/licenses/MIT
[appplant]: www.appplant.de
