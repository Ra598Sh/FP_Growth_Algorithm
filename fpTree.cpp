#include<bits/stdc++.h>
using namespace std;
struct string_frequency
{
    int count;
    string str;
};

typedef struct tree
{
	string item;
	int count;
	tree *next[1000];
}tree;
typedef tree* eptr;

int customers()
{
    int count=0;
    ifstream file;
    file.open ("data.txt");
    string line;
    while(getline(file,line))
    {
        count++;
    }
    return count;
}

void readFile(map<string,int> &temp)
{
    // READING WORDS FROM FILE AND OBTAINING FREQUENCY 
    ifstream file;
    file.open ("data.txt");
    if (!file.is_open())
    {
        return;
    }
    string word;
    while (file >> word)
    {
        if(temp.count(word)==0)
        {
            temp.insert(pair<string,int>(word,1));
        }
        else
        {
            temp[word]++;
        }
    }
}

void customer_items_in_map(map <string,int> temp[],int n)
{
    ifstream file;
    file.open ("data.txt");
    string line;
    string word;
    int i=0;
    while(getline(file,line))
    {
        //read new line of file
        istringstream iss(line);
        while(iss >> word)
        {
            //read words of line
            temp[i].insert(pair<string,int>(word,1));
        }
        i++;
    }
}

void valid_items(map <string,int> individual_customer[],map <int,string> valid_customer_item[],int n,struct string_frequency arr[],int map_size)
{
    int i,j,k=0;
    for(i=0;i<n;++i)
    {
        for(j=0;j<map_size;++j)
        {   
            if(individual_customer[i].count(arr[j].str)==1)
            {
                valid_customer_item[i].insert(pair<int,string>(k,arr[j].str));
                k++;
            }
        }
    }
}

bool method(struct string_frequency a,struct string_frequency b)
{
    //method to sort structure (on basis of frequency of strings)
    if(a.count>b.count)
    {
        return 1;
    }
    return 0;
}

eptr newN()
{
	int i;
	eptr newNode=new tree;
	newNode->item='0';
	for(i=0;i<1000;++i)
		newNode->next[i]=NULL;
	newNode->count=0;
	return newNode;	
}

eptr newN(string ch)
{
	int i;
	eptr newNode=new tree;
	newNode->item=ch;
	//cout<<newNode<<endl;
	for(i=0;i<1000;++i)
		newNode->next[i]=NULL;
	newNode->count=1;
	//cout<<newNode<<endl;
	return newNode;
}

eptr fp_tree(map <int,string> valid_customer_item[],map <string,int> check_in,int total_customers)
{
	int i;
    //cout<<total_customers<<endl;
	map <int,string>::iterator it;
    // for(i=0;i<total_customers;i++)
    // {
    //     for(it=valid_customer_item[i].begin();it!=valid_customer_item[i].end();it++)
    //     {
    //         cout<<it->first<<"->"<<it->second<<' ';
    //     }
    //     cout<<endl;
    // }
    eptr root=NULL,start=NULL;
    for(i=0;i<total_customers;i++)
    {
    	root=start;
    	for(it=valid_customer_item[i].begin();it!=valid_customer_item[i].end();it++)
    	{
    		//cout<<it1->second<<" ";
    		if(it->first==0)
    		{
    			root=newN();
				start=root;
				root->next[check_in[it->second]]=newN(it->second);
				root=root->next[check_in[it->second]];
    		}
    		else
			{
				if(root->next[check_in[it->second]]!=NULL)
				{
					//cout<<root->next[check_in[it->second]]->item<<endl;
					++root->next[check_in[it->second]]->count;
					root=root->next[check_in[it->second]];
				}
				else
				{
					//cout<<it1->second<<endl;
					root->next[check_in[it->second]]=newN(it->second);
					root=root->next[check_in[it->second]];
				}
				//cout<<root<<endl;
			}
    	}
    }
    return start;
}

void common(eptr root,map <string,int>check_in,int map_size,map <string,int>frequent[],string array_of_items[],int i)
{
    int j=0,index=0,k=0;
    //cout<<root<<" "<<i<<endl;
    if(root==NULL)
        return;
    if(i!=-1)
    {
    	//cout<<check_in[root->item]<<" "<<root->item<<" "<<i<<endl;
        //cout<<"yes\n";
        index=check_in[root->item];
        for(k=0;k<i;++k)
        {
        	//cout<<array_of_items[k]<<endl;
           	if(frequent[index].count(array_of_items[k])==0)
                frequent[index].insert(pair<string,int>(array_of_items[k],1));
            else
                ++frequent[index][array_of_items[k]];
        }
        array_of_items[i]=root->item;
        //cout<<check_in[root->item]<<" "<<root->item<<" "<<i<<endl;
    }
    for(j=1;j<=map_size;++j)
    {
        if(root->next[j]!=NULL)
        {
        	//cout<<root->next[j]->item<<" "<<root->next[j]->count<<endl;
            common(root->next[j],check_in,map_size,frequent,array_of_items,i+1); 
            //cout<<"ret"<<endl;
        }
    }
}

void print(eptr root,int map_size)
{
	int i;
	if(root==NULL)
		return;
	for(i=1;i<=map_size;++i)
		if(root->next[i]!=NULL)
		{
			cout<<root->next[i]->item<<" "<<root->next[i]->count<<endl;
			print(root->next[i],map_size);
		}
	return;
}

int main()
{
    int map_size,i,total_customers,min_support=3;
    eptr start;

    map <string,int>::iterator it;
    map <string,int> frequency;

    readFile(frequency);
    map_size=frequency.size();
    struct string_frequency arr[map_size];

    total_customers=customers();
    map <string,int> individual_customer[total_customers];
    map <int,string> valid_customer_item[total_customers];
    map <string,int> frequent[map_size+10];

    string array_of_items[total_customers];
    //int total_customers; 
    //total_customers;
    for (i=0,it=frequency.begin();it!=frequency.end();it++,i++)
    {
        arr[i].str=it->first;
        arr[i].count=it->second;
    }
    // sort strings on basis of frequency
    sort(arr,arr+map_size,method);
    map <string,int> check_in;
    for(i=0;i<map_size;i++)
    {
        //cout<<arr[i].str<<"->"<<arr[i].count<<endl;
        if(arr[i].count>=min_support)
        	check_in.insert(pair<string,int>(arr[i].str,i+1));
        else
        {
            map_size=i;
            break;
        }
    }
    /*for(i=1,it=check_in.begin();i<=map_size;++i,++it)
    {
    	cout<<it->first<<" ";
    	cout<<check_in[it->first]<<endl;
    }*/
    customer_items_in_map(individual_customer,total_customers);
    //cout<<1<<endl;
    valid_items(individual_customer,valid_customer_item,total_customers,arr,map_size);
    //cout<<2<<endl;
    start=fp_tree(valid_customer_item,check_in,total_customers);
    //cout<<3<<endl;
    common(start,check_in,map_size,frequent,array_of_items,-1);
    //cout<<4<<endl;
    print(start,map_size); 
    //cout<<5<<endl;
    //cout<<total_customers<<endl;
    /*for(i=0;i<total_customers;i++)
    {
        for(it=individual_customer[i].begin();it!=individual_customer[i].end();it++)
        {
            cout<<it->first<<"->"<<it->second<<' ';
        }
        cout<<endl;
    }*/
    return 0;
}
