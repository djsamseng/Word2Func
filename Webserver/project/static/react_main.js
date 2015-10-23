//ReactDOM.render(<h1>Hello, world!</h1>, document.getElementById('mount_react'));

var SearchForm = React.createClass({
    handleSubmit: function(e) {
        e.preventDefault();
        var searchText2 = this.refs.searchText.value.trim();
        this.props.onSearchSubmit({searchText: searchText2});
        return;
    },
    render: function() {
        return (
            <div>
                <form className="searchForm" onSubmit={this.handleSubmit}>
                    <input type="text" placeholder="Machine Learning" ref="searchText" />
                    <input type="submit" value="Search" />
                </form>
            </div>
        );
    }
});

var SearchResultList = React.createClass({
    render: function() {
        return (
            <div className="searchResultList">
                <ul>
                    {this.props.data.map(function(link) {
                        return (
                            <li key={link.id}>
                                <p>{link.title}</p>
                                <p>{link.snippet}</p>
                                <a href={link.link}>{link.link}</a>
                            </li>
                        );
                    })}
                </ul>
            </div>
        );
    }
});

var SearchResults = React.createClass({
    //Sets initial state
    getInitialState: function() {
        return { links: initLinks }//from static/CONSTANTS
    },
    handleSearchSubmit: function(data) {
        $.ajax({
            url: '/get_links',
            method: 'POST',
            data: JSON.stringify({ 'query':data.searchText}, null, '\t'),
            contentType: 'application/json;charset=UTF-8',
            success: function(data) {
                console.log(data);
                data = JSON.parse(data);
                this.setState({ links: data });
            }.bind(this),
            error: function(xhr, status, err) {
                console.error('/get_links', status, err.toString());
            }.bind(this)
        });
    },
    render: function() {
        return (
            <div className="searchResults">
                <SearchForm onSearchSubmit={this.handleSearchSubmit} />
                <SearchResultList data={this.state.links} />
            </div>
        );
    }
});

ReactDOM.render(
    <SearchResults />,
    document.getElementById('mountReact')
);

