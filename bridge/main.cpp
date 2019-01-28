#include <chrono>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Semaphore
{
public:
    Semaphore(unsigned long count)
        : count_{count}
    {
    }

    // Not copyable.
    Semaphore(Semaphore const &) = delete;
    Semaphore & operator=(Semaphore const &) = delete;

	void signal()
	{
        std::unique_lock lock{mutex_};
        ++count_;
        condition_.notify_one();
	}

	void wait()
    {
        std::unique_lock lock{mutex_};
        while (not count_)
        {
            condition_.wait(lock);
        }
        --count_;
    }

    bool try_wait()
    {
        std::lock_guard lock{mutex_};
        if (count_)
        {
            --count_;
            return true;
        }
        return false;
    }

private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_;
};

class Bridge
{
public:
    void cross(std::string const & who)
    {
        std::cout << who << " wants to cross the bridge" << std::endl;
        semaphore_.wait();
        std::cout << who << " is crossing the bridge" << std::endl;
        semaphore_.signal();
    }
private:
    Semaphore semaphore_ = Semaphore{1};
};

class Farmer
{
public:
    Farmer(std::string const & name, Bridge & bridge)
        : name_{name}
        , bridge_{bridge}
    {
    }

    void cross()
    {
        bridge_.cross(name_);
    }

private:
    std::string name_;
    Bridge & bridge_;
};

void produce_farmers(std::string const & lane, Bridge & bridge)
{
    using namespace std::chrono_literals;

    // Keep producing farmers to cross the bridge after some delay.
    while (true)
    {
        auto farmer = std::make_unique<Farmer>(lane, bridge);
        std::thread th{
            [farmer=std::move(farmer)]()
            {
                farmer->cross();
            }};
        th.detach();

        std::this_thread::sleep_for(1s);
    }
}

int main(int argc, char * argv[])
{
    std::ios_base::sync_with_stdio(false);

    // Shared bridge.
    Bridge bridge{};

    // Produce farmers on each side of the bridge.
    std::thread northbound_thread{
        [&bridge]()
        {
            produce_farmers("North", bridge);
        }};
    std::thread southbound_thread{
        [&bridge]()
        {
            produce_farmers("South", bridge);
        }};

    northbound_thread.join();
    southbound_thread.join();
	return EXIT_SUCCESS;
}
