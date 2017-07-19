# return a list of exactly len(rebootTimes) lists
# result[k] -> list of job indexes for server k


# 30 minutes to read problem and write this code


rebootTimes = [ 30, 45, 20]
jobTimes = [ 30, 45, 20, 30, 15, 10, 20, 15, 40, 20, 25, 30, 10, 15, 30, 20, 5, 35]

def schedule_jobs(rebootTimes, jobTimes):
        
    print(type(rebootTimes))
    print(len(rebootTimes))
    print(rebootTimes)
    print()
    
    print(type(jobTimes))
    print(len(jobTimes))
    print(jobTimes)
    print()
    
    class server():
    
        def __init__(self):
            
            self.id = None
            self.job_times = []
            self.job_ids = []
            self.capacity = None
            
    
    rebootTimes.sort(reverse=True)
    jobTimes.sort(reverse=True)
    
    sorted_server_object_list = []
    
    # 1. assign all servers
    for i, s_capacity in enumerate(rebootTimes):
        
        s = server()
        s.id = i
        s.capacity = s_capacity
        sorted_server_object_list.append(s)
        
        
    # 2. assign all jobs
    # TODO , tighter loop to handle jobs that don't meet next server available time

    len_servers = len(rebootTimes)
    for i, j in enumerate(jobTimes):
        
        # get next server
        index = i % len_servers
        s = sorted_server_object_list[index] 
        
        print(j, s.capacity)
        if j <= s.capacity:
            s.job_times.append(j) 
            s.job_ids.append(i)
               
    # 3. format output
    output = []
    for s in sorted_server_object_list:
        output.append((s.job_ids))
    
    print()
    print(jobTimes)
    print(output)
    print(rebootTimes)
    
    return output


output = schedule_jobs(rebootTimes, jobTimes)

print("\nOutput:\n", output)