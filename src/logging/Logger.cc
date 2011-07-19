#include <Logging/Logger.hh>
#include <Logging/Message.hh>

namespace dadi {

const std::string Logger::root_;
LoggerMap Logger::lmap_;

Logger::Logger(const std::string& name,
               ChannelPtr channel,
               int level)
  : name_(name), channel_(channel), level_(level) {}

const std::string&
Logger::getName() const {
  return name_;
}

void
Logger::setChannel(ChannelPtr channel) {
  channel_ = channel;
}

ChannelPtr
Logger::getChannel() const {
  return channel_;
}

void
Logger::setLevel(int level) {
  level_ = level;
}

int
Logger::getLevel() const {
  return level_;
}

void
Logger::log(const Message& msg) {
  if ((msg.getPriority() >= level_) &&
      channel_) {
    channel_->log(msg);
  }
}

bool
Logger::is(int level) const {
  return (level_ <= level);
}

bool
Logger::trace() const {
  return is(Message::PRIO_TRACE);
}

bool
Logger::debug() const {
  return is(Message::PRIO_DEBUG);
}

bool
Logger::information() const {
  return is(Message::PRIO_INFORMATION);
}

bool
Logger::warning() const {
  return is(Message::PRIO_WARNING);
}

bool
Logger::error() const {
  return is(Message::PRIO_ERROR);
}

bool
Logger::fatal() const {
  return is(Message::PRIO_FATAL);
}

LoggerPtr
Logger::getRootLogger() {
  return get(root_);
}

LoggerPtr
Logger::getLogger(const std::string& name) {
  return get(name);
}

LoggerPtr
Logger::createLogger(const std::string& name,
                     ChannelPtr channel,
                     int level = Message::PRIO_INFORMATION) {
  if (find(name)) {
    throw std::string("Already exists");
  }

  LoggerPtr newLogger(new Logger(name, channel, level));
  add(newLogger);

  return newLogger;
}

void
Logger::shutdown() {
  lmap_.clear();
}


LoggerPtr
Logger::find(const std::string& name) {
  LoggerMap::iterator it = lmap_.find(name);
  if (lmap_.end() != it) {
    return it->second;
  }

  return LoggerPtr();
}

LoggerPtr
Logger::get(const std::string& name) {
  LoggerPtr logger = find(name);

  if (!logger) {
    if (root_ == name) {
      logger.reset(new Logger(name, LoggerPtr(), Message::PRIO_INFORMATION));
    } else {
      LoggerPtr parent = getParent(name);
      logger.reset(new Logger(name, parent->getChannel(), parent->getLevel()));
    }
    add(logger);
  }

  return logger;
}


void
Logger::add(LoggerPtr logger) {
  lmap_.insert(LoggerMap::value_type(logger->getName(), logger));
}

LoggerPtr
Logger::getParent(const std::string& name) {
  std::string::size_type pos = name.rfind('.');
  if (std::string::npos != pos) {
    std::string pname = name.substr(0, pos);
    LoggerPtr parent = find(pname);
    if (parent) {
      return parent;
    } else {
      // logger re-parented to grand-parent
      return getParent(pname);
    }
  } else {
    return get(root_);
  }
}

} /* namespace dadi */